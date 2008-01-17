/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.parser;

import org.xml.sax.SAXParseException;


import rpn.RPnMenuCommand;
import rpnumerics.RPNumericsProfile;
import wave.util.RealVector;
import rpnumerics.RpNumerics;
import rpnumerics.PhasePoint;
import org.xml.sax.HandlerBase;
import org.xml.sax.AttributeList;
import org.xml.sax.SAXException;
import org.xml.sax.Parser;
import org.xml.sax.InputSource;
import java.io.FileWriter;
import java.util.StringTokenizer;
import java.io.InputStream;
import java.util.ArrayList;
import wave.util.RectBoundary;
import wave.util.IsoTriang2DBoundary;

/** This class implements methods to configure the numeric layer. The values are taked from a XML file and this values are used to setup the physics and all others numerics parameters. */

public class RPnNumericsModule {
    //
    // Constants
    //
    private static RPNumericsProfile profile_ = new RPNumericsProfile();
    private static PhasePoint bufferedPhasePoint_;
    private static double sigma_;
    private static RPnMenuCommand command_;
    
    static class InputHandler extends HandlerBase {
        //
        // Members
        //
        private RealVector tempVector_;
        private String currentElement_;
        
        private ArrayList boundsVectorArray_;
        
        public void startElement(String name, AttributeList att) throws
                SAXException {
            
            currentElement_ = name;
            
            if (name.equals("NUMERICS")) {
                
            }
            
            if (name.equals("PHYSICS")) {
                profile_.initPhysics(att.getValue(0),att.getValue(1),att.getValue(2));
            }
            
            if (name.equals("BOUNDARY")) {
                int boundsNumber = new Integer(att.getValue(0)).intValue();
                boundsVectorArray_ = new ArrayList(boundsNumber);
            }
            
            if (name.equals("CURVE")) {
                
                StringTokenizer tokenizer = new StringTokenizer(att.getValue(1));
                int [] variations = new int[tokenizer.countTokens()];
                int i = 0;
                while (tokenizer.hasMoreTokens()) {
                    variations[i]= (new Integer(tokenizer.nextToken())).intValue();
                    i++;
                }
            }
            
            if (name.equals("FLOWTYPE")) {
                profile_.initFlowType(att.getValue(0));
//                profile_.setFamilyIndexFlow(new Integer(att.getValue(1)));
//                System.out.println("Valor do familyIndex apos parser: "+ profile_.getFamilyIndex());
                
            }
            
            if (name.equals("RAREFACTIONFLOW")) {
                
                profile_.setFamilyIndexFlow((new Integer(att.getValue(0)).intValue()));
            }
            
            if (name.equals("FLUXPARAMS")) {
                StringTokenizer tokenizer = new StringTokenizer(att.getValue(0));
                double doubleList[] = new double[tokenizer.countTokens()];
                int i = 0;
                while (tokenizer.hasMoreTokens()) {
                    doubleList[i++] = new Double(tokenizer.nextToken()).
                            doubleValue();
                }
                
                RpNumerics.changeFluxParams(doubleList);
            }
            
            if (name.equals("PHASEPOINT")) {
                
                tempVector_ = new RealVector((new Integer(att.getValue(0))).
                        intValue());
                
            }
            if (name.equals("SHOCKFLOWDATA")) {
                
                sigma_ = new Double(att.getValue(0)).doubleValue();
                
            }
            
        }
        
        public void characters(char[] buff, int offset, int len) throws
                SAXException {
            
            String data = new String(buff, offset, len);
            data = data.trim();
            if (data.length() != 0) {
                
                if (currentElement_.equals("PHASEPOINT")) {
                    StringTokenizer tokenizer = new StringTokenizer(data);
                    int i = 0;
                    while (tokenizer.hasMoreTokens()) {
                        tempVector_.setElement(i,
                                (new Double(tokenizer.nextToken()).
                                doubleValue()));
                        i++;
                    }
                    bufferedPhasePoint_ = new PhasePoint(tempVector_);
                }
                
                if (currentElement_.equals("REALVECTOR")) {
                    tempVector_ = new RealVector(data);
                    boundsVectorArray_.add(tempVector_);
                    
                }
                
            }
            
        }
        public void endElement(String name) throws SAXException {
            if (name.equals("PHYSICS")) {
                
                try {
                    RpNumerics.init(profile_);
                    
                } catch (rpnumerics.RpException e) {
                    e.printStackTrace();
                }
            }
            
            if (name.equals("BOUNDARY")) {
                
                if (boundsVectorArray_.size() == 2) {
                    profile_.setBoundary(new RectBoundary((RealVector)boundsVectorArray_.get(0),(RealVector)boundsVectorArray_.get(1)));
                }
                
                if (boundsVectorArray_.size() == 3) {
                    profile_.setBoundary(new IsoTriang2DBoundary((RealVector)boundsVectorArray_.get(0),(RealVector)boundsVectorArray_.get(1),(RealVector)boundsVectorArray_.get(2)));
                }
            }
            
            if (name.equals("CURVE")) {
                
                
            }
            
            if (name.equals("SHOCKFLOWDATA")) {
                
                RpNumerics.setShockFlow(bufferedPhasePoint_, sigma_);
                
            }
            
        }
        
    }
    
    
    //
    // Members
    //
    //
    // Initializers
    //
    
    
    public static void init(Parser parser, String file) {
        try {
            parser.setDocumentHandler(new InputHandler());
            parser.parse(file);
        } catch (Exception saxex) {
            saxex.printStackTrace();
        }
    }
    
    public static void init(Parser parser, InputStream configFileStream) {
        try {
            
            parser.setDocumentHandler(new InputHandler());
            System.out.println("Numerics Module");
            
            System.out.println("Will parse !");
            parser.parse(new InputSource(configFileStream));
            
            System.out.println("parsed !");
            
        } catch (Exception saxex) {
            
            if (saxex instanceof SAXParseException) {
                
                SAXParseException e = (SAXParseException) saxex;
                
                System.out.println("Line error: " + e.getLineNumber());
                System.out.println("Column error: " + e.getColumnNumber());
            }
            
            saxex.printStackTrace();
        }
    }
    
    //
    // Methods
    //
    
    public static void setCommand(RPnMenuCommand command) {
        
        command_ = command;
        
    }
    
    /** Writes the actual values of the numerics parameters into a XML file. */
    
    public static void export(FileWriter writer) throws java.io.IOException {
        
    }
}
