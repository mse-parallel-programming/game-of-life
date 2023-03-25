package ppr.messages;

import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.ObjectCodec;
import com.fasterxml.jackson.core.TreeNode;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.ArrayNode;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-25
 */
public class DiffDeserializer extends JsonDeserializer<Map<Integer, List<Integer>>> {
    @Override
    public Map<Integer, List<Integer>> deserialize(
        JsonParser p, DeserializationContext ctxt
    ) throws IOException, JacksonException {
        // Based on https://stackoverflow.com/a/39012861
        // Reason: https://github.com/nlohmann/json/issues/1664
        var retVal = new HashMap<Integer, List<Integer>>();

        ObjectCodec codec = p.getCodec();
        TreeNode node = codec.readTree(p);

        if (node.isArray()) {
            for (JsonNode n : (ArrayNode)node) {
                if (!n.isArray()) continue;
                var row = n.get(0).asInt();
                var indices = new ArrayList<Integer>();
                var iter = n.get(1).elements();
                while (iter.hasNext())
                    indices.add(iter.next().asInt());

                retVal.put(row, indices);
            }
        }

        return retVal;
    }
}
