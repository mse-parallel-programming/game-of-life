package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;

import java.util.List;
import java.util.Optional;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
@JsonInclude(JsonInclude.Include.NON_NULL)
public class StartMessage {

    public GameInput input;
    public BenchmarkInput benchmarkInput;
}