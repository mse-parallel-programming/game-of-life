package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import ppr.messages.helper.DiffDeserializer;

import java.util.List;
import java.util.Map;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-25
 */
@JsonInclude(JsonInclude.Include.NON_NULL)
public class UpdateMessage {
    public int generation;
    @JsonDeserialize(using = DiffDeserializer.class)
    public Map<Integer, List<Integer>> diff;

    @Override
    public String toString() {
        return "UpdateMessage(generation=" + generation + ", diff=" + diff + ")";
    }

    public void updateGrid(List<List<Boolean>> grid) {
        for (Map.Entry<Integer, List<Integer>> entry: diff.entrySet()) {
            var row = grid.get(entry.getKey());
            for (Integer index : entry.getValue()) {
                var newVal = !row.get(index);
                row.set(index, newVal);
            }
        }
    }
}
