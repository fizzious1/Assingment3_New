//Added by Ariel 04/01/23
package bgu.spl.net.srv;

public enum FrameHeader {
    ACCEPT_VERSION("accept-version"),
    CONTENT_TYPE("content-type"),
    DESTINATION("destination"),
    HOST("host"),
    ID("id"),
    MESSAGE_ID("message-id"),
    RECEIPT_ID("receipt-id"),
    RECEIPT("receipt"),
    SUBSCRIPTION("subscription"),
    VERSION("version");
    final String value;
    private FrameHeader(String value){
        this.value = value;
    }
    public String toString(){
        return this.value;
    }
}
