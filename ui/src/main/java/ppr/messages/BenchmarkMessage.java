package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;

import java.util.List;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-27
 */

@JsonInclude(JsonInclude.Include.NON_NULL)
public class BenchmarkMessage {
    public List<List<Boolean>> grid;
    public double averageTime;
    public double minTime;
    public double maxTime;
}
