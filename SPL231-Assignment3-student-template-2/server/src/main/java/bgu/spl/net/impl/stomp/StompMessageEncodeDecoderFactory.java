package bgu.spl.net.impl.stomp;

import java.util.function.Supplier;

import bgu.spl.net.api.MessageEncoderDecoder;

public class StompMessageEncodeDecoderFactory implements Supplier<MessageEncoderDecoder<String>>{
    
    @Override
    public MessageEncoderDecoder<String> get() {
        return new StompMessageEncoderDecoder();
    }
    
}
