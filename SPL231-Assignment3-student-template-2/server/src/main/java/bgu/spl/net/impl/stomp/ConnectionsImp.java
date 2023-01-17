package bgu.spl.net.impl.stomp;

import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;

import bgu.spl.net.srv.Connections;

public class ConnectionsImp implements Connections<String> {
    HashMap<Socket, StompConnectionHandler<String>> activeConnections = new HashMap<>();
    @Override
    public boolean send(int connectionId, String msg) {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void send(String channel, String msg) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void disconnect(int connectionId) {
        // TODO Auto-generated method stub
        
    }
 
    
    
}
