import java.io.*;

public class Bai4 {
    public static void main(String[] args) throws IOException {
        FileOutputStream out = null;
        FileInputStream in = null;

        try {
            in = new FileInputStream("Bai4.png");
            out = new FileOutputStream("Bai4_copy.png");

            System.out.println("Start reading file!");

            int c;
            while ((c = in.read()) != -1) {
                    out.write(c);
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
