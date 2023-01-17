//Added by Ariel 04/01/23 frame reader and making new frame to send
package bgu.spl.net.impl.stomp;
import java.util.HashMap;
import java.util.Map;
public class Frame {
      FrameCommand command; 
     Map<String, String> headers = new HashMap<>();
    final  StringBuilder body;
      StringBuilder frame; // the whole frame
     String[] lines; //each line examined seperatly
     String data; //input data from the user
     int counter = 0; //indicate which index is lines[] first non-empty line
     //Initial Constructor 
     public Frame(String data){ //we only get the data from client/server
        this.data = data;
        command = null;
        body = null;
        frame = null;
     }
    
    public void linesSep(){ //seperate lines 
       lines = data.split("\n");
    }
    public void skipEmptyLines(){
        while(counter<lines.length && lines[counter].isEmpty()) // while lines is empty, move to next line
        counter++;
    }
     void identifyCommand(){ //what instance of frame will it be
         command = FrameCommand.findCommand(lines[counter]);
        counter++;
        
     }
     
     void identifyHeaders(){ //extract headers from data
        while(!lines[counter].isEmpty()){
            String[] val = lines[counter].split(":"); //
            headers.put(val[0], val[1]);
            counter++;

        }}
        void buildBody(){
            for(int i = counter ; i < lines.length ; i++)
            body.append(lines[counter]);
        }
        void buildFrame(){
            frame.append(command);
        frame.append('\n');
        // Headers
        for (String currHeader : headers.keySet()) {
            frame.append(currHeader);
            frame.append(':');
            frame.append(headers.get(currHeader));
            frame.append('\n');
        }
        frame.append('\n');
        // Body
        if (body != null) {
            frame.append(body);
        }
        }
        public void build(){
            linesSep();
            skipEmptyLines();
            identifyCommand();
            buildBody();
            buildFrame();
        }
        }
     
   

