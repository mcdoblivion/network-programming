import java.io.*;

public class Bai3 {
    public static void main(String[] args) throws IOException {
        FileOutputStream out = null;
        FileInputStream in = null;

        try {
            in = new FileInputStream("Bai3.txt");
            out = new FileOutputStream("Bai3_copy.txt");

            System.out.println("Start reading file!");

            int c;
            while ((c = in.read()) != -1) {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    out.write(c);
                }
            }

            System.out.println("Finish reading file!");
        } finally {
            if (out != null) {
                out.close();
            }
            if (in != null) {
                in.close();
            }
        }
    }
}
