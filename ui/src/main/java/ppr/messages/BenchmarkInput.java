package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
@JsonInclude(JsonInclude.Include.NON_NULL)
public class BenchmarkInput {
    public int iterations;
    public int generations;
    public Boolean dynamic; // Nullable
    public Integer threadCount; // Nullable
}