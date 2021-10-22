import java.io.*;
import java.util.*;

public class Bai1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Please enter something:");
        String input = scanner.nextLine();
        scanner.close();

        char[] chars = input.toCharArray();

        int digit = 0, letter = 0;
        for (char c : chars) {
            if ((c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')) {
                letter++;
            } else if ((c >= '0' && c <= '9')) {
                digit++;
            }
        }
        System.out.printf("You have just entered: %s\n, length: %d\n", input, input.length());
        System.out.printf("Number of digits: %d\n", digit);
        System.out.printf("Number of letters: %d\n", letter);
        System.out.printf("Number of symbols: %d\n", input.length() - digit - letter);
    }
}