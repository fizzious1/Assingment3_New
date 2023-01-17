package bgu.spl.net.impl.stomp;

import java.util.function.Supplier;

public class StompMessageProtocolFactory implements Supplier<StompMessageProtocol> {

    @Override
    public StompMessageProtocol get() {
        return new StompMessageProtocol();
    }
    
}
