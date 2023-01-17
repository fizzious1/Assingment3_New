//Added by Ariel 04/01/23
package bgu.spl.net.impl.stomp;
//Imp for client side
public enum FrameCommandClientSide {
    CONNECT("login"),
    CONNECTED("CONNECTED"),
    DISCONNECT("logout"),
    ERROR("ERROR"),
    MESSAGE("MESSAGE"),
    RECEIPT("RECEIPT"),
    SEND("report"),
    SUBSCRIBE("join"),
    UNSUBSCRIBE("exit");
    public String value;

    private FrameCommandClientSide(String value){
    this.value = value;
}
public String toString(){
    return this.value;
}
public static FrameCommandClientSide findCommand(String value){
    for(FrameCommandClientSide finder : FrameCommandClientSide.values()){
        if(finder.value.equals(value))
        return finder;
    }
    throw new IllegalArgumentException("command unkown");
}
}
   