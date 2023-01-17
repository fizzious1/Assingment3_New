package bgu.spl.net.impl.stomp;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.channels.Selector;
import java.util.function.Supplier;

import bgu.spl.net.srv.Server;

public class StompServer<T> implements Server<T>{
    int port;
    ConnectionsImp connections = new ConnectionsImp();
    Supplier<StompMessageEncoderDecoder> encoderDecoderFactory;
    Supplier<StompMessageProtocol> protocolFactory;
    ServerSocket sock;
    String message;
    int nThreads;
    Selector selector;
    boolean isReactor;
    public StompServer(int port, Supplier<StompMessageProtocol> protocolFactory,
    Supplier<StompMessageEncoderDecoder> encdecFactory){
        this.port = port;
        this.encoderDecoderFactory = encdecFactory;
        this.protocolFactory = protocolFactory;
    }
    
    }
    public static void main(String[] args) {
        int port = 8000;
        StompMessageEncodeDecoderFactory encDecFactory = new StompMessageEncodeDecoderFactory();
        StompMessageProtocolFactory protocolFactory = new StompMessageProtocolFactory();
        //StompServer server = new StompServer(port, protocolFactory, encDecFactory);
    }
    @Override
    public void close() throws IOException {
        // TODO Auto-generated method stub
        
    }
    @Override
    public void serve() {
        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close
            Socket clientSock = serverSock.accept(); //create new socket for client

                StompConnectionHandler<T> handler = new StompConnectionHandler<>(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get());

                execute(handler);
                connections.activeConnections.put(clientSock, handler);
                connections.send(message, message);


        
    }
}
