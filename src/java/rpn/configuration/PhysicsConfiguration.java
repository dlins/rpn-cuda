/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.configuration;

import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;

public class PhysicsConfiguration extends Configuration{


    public PhysicsConfiguration(ConfigurationProfile configurationProfile,HashMap <String,Configuration> configurationHash ){
        
        super(configurationProfile,configurationHash);

    }
    
    public PhysicsConfiguration(String name){
        
        super(name);
        
    }
    
    
    
    
 
    @Override
    public String toXML() {

        StringBuilder buffer = new StringBuilder();
        
        Set<Entry<String, String>> paramsSet = getParams().entrySet();

            buffer.append("<COMMAND name=\"").append(getName()).append("\" ");

            for (Entry<String, String> entry : paramsSet) {
                buffer.append(entry.getKey()).append("=\"").append(entry.getValue()).append("\"");
                buffer.append(" ");
            }

            buffer.append("/>");

        return buffer.toString();
    }
}
