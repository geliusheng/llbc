# -*- coding: utf-8 -*-
"""
Service basic 测试
"""
from llbc import TestCase, facade, packet, bindto, Service, Stream, handler, exc_handler

@facade
@bindto('svcbase_test_svc')
class TestFacade(object):
    def __init__(self):
        self._call_times = 0

    def oninitialize(self, ev):
        print 'service init: {}'.format(ev)

    def onstart(self, ev):
        print 'service start: {}'.format(ev)

    def onstop(self, ev):
        print 'service stop: {}'.format(ev)

    def ondestroy(self, ev):
        print 'service destroy: {}'.format(ev)

    def onupdate(self, ev):
        # print 'service update: {}'.format(d)
        self._call_times += 1
        if self._call_times == 1000:
            print 'Service update {} times, deschedule it'.format(self._call_times)
            Service.deschedule()

    def onidle(self, ev):
        pass
        # print 'service idle: {}'.format(d)

    def onsessioncreate(self, ev):
        print 'new session create: {}'.format(ev)

    def onsessiondestroy(self, ev):
        print 'session destroy, {}'.format(ev)

    def onasyncconnresult(self, ev):
        print 'async-conn result: {}'.format(ev)

    def onunhandledpacket(self, ev):
        print 'unhandled packet: {}'.format(ev)

    def onprotoreport(self, ev):
        print 'protocol report: {}'.format(ev)


@packet
@bindto('svcbase_test_svc')
class TestData(object):
    def __init__(self):
        self.iVal = 3
        self.strVal = 'hello pyllbc'
        self.listVal = [1, 3, 4, 5]
        self.dictVal = {1: 'hello', 2: 'python'}

    def encode(self, s):
        assert isinstance(s, Stream)
        s.pack('iS[i]{i:S}', self.iVal, self.strVal, self.listVal, self.dictVal)

    def decode(self, s):
        assert isinstance(s, Stream)
        self.iVal, self.strVal, self.listVal, self.dictVal = s.unpack('iS[i]{i:S}')


@handler(TestData)
@bindto('svcbase_test_svc')
class TestHandler(object):
    def __call__(self, packet):
        svc = packet.svc
        data = packet.data
        session_id = packet.session_id
        print 'session[{}] recv data, opcode:{}, data:{}'.format(session_id, packet.opcode, data)

        print 'send response...'
        svc.send(session_id, data)
        # raise Exception('Test exception, raise from TestData packet handler')

@exc_handler(TestData)
@bindto('svcbase_test_svc')
class ExcHandler(object):
    def __call__(self, svc, tb, e):
        print 'Exc handler, tb: {}, e: {}'.format(tb, e)

class SvcBaseTest(TestCase):
    def misc_test(self):
        s = '11111111'
        f = open('aaa')
        f.seek(0)
        s.count('1')

    def run(self, arg):
        print 'Service base test:'
        # Create service: my_svc
        svc = Service('svcbase_test_svc')
        svc.codec = Service.CODEC_BINARY
        svc.start()

        # Listen
        ip = '127.0.0.1'
        port = 7788
        listen_sid = svc.listen(ip, port)
        print 'Service {} listening in {}:{}, session Id: {}'.format(svc.name, ip, port, listen_sid)

        # Connect to listen session.
        conn_sid = svc.connect(ip, port)
        print 'Connect to {}:{} success, sid: {}'.format(ip, port, conn_sid)

        # Send data.
        svc.send(conn_sid, TestData())
        # Test unhandled packet.
        svc.send(conn_sid, data=3, opcode=10086, status=0)

        # Create service: another_svc
        another = Service('another')
        another.codec = Service.CODEC_BINARY
        another.start()

        # Schedule.
        Service.schedule()

        # If deschedule, will gone to here.
        # Delete my_svc service.
        del Service.svcbase_test_svc
        del Service.another
