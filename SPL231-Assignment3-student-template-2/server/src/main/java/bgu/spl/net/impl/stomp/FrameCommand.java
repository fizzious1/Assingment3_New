package bgu.spl.net.impl.stomp;

public enum FrameCommand {
    CONNECT("CONNECT"),
    CONNECTED("CONNECTED"),
    DISCONNECT("DISCONNECT"),
    ERROR("ERROR"),
    MESSAGE("MESSAGE"),
    RECEIPT("RECEIPT"),
    SEND("SEND"),
    SUBSCRIBE("SUBSCRIBE"),
    UNSUBSCRIBE("UNSUBSCRIBE");
    public String value;

    private FrameCommand(String value){
    this.value = value;
}
public String toString(){
    return this.value;
}
public static FrameCommand findCommand(String value){
    for(FrameCommand finder : FrameCommand.values()){
        if(finder.value.equals(value))
        return finder;
    }
    throw new IllegalArgumentException("command unkown");
}
}
