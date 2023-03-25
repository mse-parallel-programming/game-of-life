package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;

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
}
