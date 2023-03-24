package ppr.util;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class Input {
    public static List<List<Boolean>> textInput(String textInput) {
        var size = (int) Math.sqrt(textInput.length());

        var input = new ArrayList<List<Boolean>>();
        for (var i = 0; i < size; ++i) {
            var row = new ArrayList<Boolean>();
            for (var j = 0; j < size; ++j) {
                var charValue = textInput.charAt((i*size)+j);
                var value = charValue == '*';
                row.add(value);
            }
            input.add(row);
        }

        return input;
    }
}
