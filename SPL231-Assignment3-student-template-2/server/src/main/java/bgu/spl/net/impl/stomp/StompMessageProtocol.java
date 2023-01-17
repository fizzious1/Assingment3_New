package bgu.spl.net.impl.stomp;

import java.util.HashMap;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.Connections;

public class StompMessageProtocol implements MessagingProtocol<String> {
    String msg;
    String replyMsg;
    int msgPointer = 0;
    int msgLength;
    Connections<String> connections;
    @Override
    public String process(String msg) { //create reply frame and send it thorugh ConnectionsImp(send)
        Frame clientFrame = new Frame(msg);
        clientFrame.build();
        FrameCommand command = clientFrame.command;
        if(command.equals(FrameCommand.CONNECT)){
            //if reactor then server.handleAccept()
            //else connect via TPC method
            Frame response = new Frame("");
            response.command = FrameCommand.CONNECTED;
            response.headers= new HashMap<>();
            response.headers.put(FrameHeader.ACCEPT_VERSION.toString(),"1.2");
            response.build();
            return response.toString();
            
        }
        else if(command.equals(FrameCommand.SUBSCRIBE)){
            //extract destination, id , receipt from frame
            //check if id is connected (through Connections) and then sign clientId to destination channel
            //generate RECEIPT frame with receipt-id  receiptValue
        }
        else if(command.equals(FrameCommand.SEND)){
            //extract destination, body from frame
            //check if id is connected && id is subscribed
            //if yes, then send msg to client/channel thorugh Connections.send(body)
        }
        else if(command.equals(FrameCommand.UNSUBSCRIBE)){
            //extract id, receipt
            //call Connections.disconnect(id)
            //generate RECEIPT frame with receipt-id  receiptValue
        }

       
       
    }

    @Override
    public boolean shouldTerminate() {
        return msg.substring(msgPointer).endsWith("\0");
    }
    void start(int connectionId, Connections<String> connections){}
    public static void main(String[] args){//tests
   // Frame frame = new Frame()
    }
}

/
        