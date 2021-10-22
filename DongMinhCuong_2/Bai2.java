import java.io.*;
import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Bai2 {
    public static void main(String[] args) throws IOException {
        FileOutputStream out = null;
        Scanner scanner = null;
        try {
            scanner = new Scanner(System.in);
            InputStreamReader cin = new InputStreamReader(System.in);

            System.out.println("Please enter filename: ");

            String fileName = scanner.nextLine();

            out = new FileOutputStream(fileName);

            System.out.println("Please enter something, press # to exit: ");

            while (true) {
                char c = (char) cin.read();
                if (c == '#')
                    break;
                out.write(c);
            }

            System.out.println("File size: " + Files.size(Paths.get(fileName)) + " bytes");

        } finally {
            if (out != null) {
                out.close();
            }
            if (scanner != null) {
                scanner.close();
            }
        }
    }
}
