package bgu.spl.net.impl.stomp;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;

public class StompConnectionHandler<T> implements ConnectionHandler<T>{
    //fields for TPC&&Reactor
    private final StompMessagingProtocol<T> protocol;
    private final StompMessageEncoderDecoder encdec;
    private volatile boolean connected = true;
    //fields for TPC
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    //fields for Reactor
    private static final int BUFFER_ALLOCATION_SIZE = 1 << 13; //8k
    private static final ConcurrentLinkedQueue<ByteBuffer> BUFFER_POOL = new ConcurrentLinkedQueue<>();
    private final Queue<ByteBuffer> writeQueue = new ConcurrentLinkedQueue<>();
    private final SocketChannel chan;

    @Override
    public void close() throws IOException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void send(T msg) {
        // TODO Auto-generated method stub
        
    }
    
}
