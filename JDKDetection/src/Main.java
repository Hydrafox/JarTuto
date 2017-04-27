import java.io.*;

/**
 * major  minor Java platform version 
 * 45       3           1.0
 * 45       3           1.1
 * 46       0           1.2
 * 47       0           1.3
 * 48       0           1.4
 * 49       0           1.5
 * 50       0           1.6
 * 51       0           1.7
 * 52       0           1.8
 */


public class Main {
public static void main(String[] args) throws IOException {

    	String filename = "D:/class/folder/TestJNI.class";
    	
    	
        DataInputStream in = new DataInputStream
         (new FileInputStream(filename));

        int magic = in.readInt();
        if(magic != 0xcafebabe) {
          System.out.println(filename + " is not a valid class!");;
        }
        int minor = in.readUnsignedShort();
        int major = in.readUnsignedShort();
        System.out.println(filename + ": " + major + " . " + minor);
        in.close();
    }
}

