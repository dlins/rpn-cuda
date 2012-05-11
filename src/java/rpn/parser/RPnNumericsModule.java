/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.parser;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;
import org.xml.sax.SAXParseException;


import wave.util.RealVector;

import org.xml.sax.SAXException;
import org.xml.sax.InputSource;
import java.io.FileWriter;
import java.io.InputStream;
import rpn.RPnConfig;
import org.xml.sax.ContentHandler;
import org.xml.sax.XMLReader;
import rpnumerics.RPNUMERICS;

/** This class implements methods to configure the numeric layer. The values are taked from a XML file and this values are used to setup the physics and all others numerics parameters. */
public class RPnNumericsModule {
    //
    // Constants
    //

    static class InputHandler implements ContentHandler {
        //
        // Members
        //

        private RealVector tempVector_;
        private String currentElement_;
        private static ConfigurationProfile currentConfigurationProfile_;
        private static ConfigurationProfile physicsProfile_;
        private static ConfigurationProfile innerPhysicsConfigurationProfile_;

        public void startElement(String uri, String localName, String qName, Attributes att) throws SAXException {
            currentElement_ = localName;

            if (localName.equals("CURVE")) {
                currentConfigurationProfile_ = new ConfigurationProfile(att.getValue("name"), ConfigurationProfile.CURVE);

            }

            if (localName.equals("CURVEPARAM")) {

                currentConfigurationProfile_.addParam(att.getValue(0), att.getValue(1));
            }

            if (localName.equals("PHYSICS")) {
                physicsProfile_ = new ConfigurationProfile(att.getValue(0), ConfigurationProfile.PHYSICS_PROFILE);
            }

            if (localName.equals("BOUNDARY")) {
                physicsProfile_.addConfigurationProfile(ConfigurationProfile.BOUNDARY, new ConfigurationProfile(att.getValue(0), ConfigurationProfile.BOUNDARY));
            }

            if (localName.equals("PHYSICSCONFIG")) {
                innerPhysicsConfigurationProfile_ = new ConfigurationProfile(att.getValue("name"), ConfigurationProfile.PHYSICS_CONFIG);
            }

            if (localName.equals("PHYSICSPARAM")) {

                innerPhysicsConfigurationProfile_.addParam(new Integer(att.getValue("position")), att.getValue("name"), att.getValue("value"));

            }

            if (localName.equals("BOUNDARYPARAM")) {

                physicsProfile_.getConfigurationProfile(ConfigurationProfile.BOUNDARY).addParam(att.getValue(0), att.getValue(1));

            }


            if (localName.equals("METHOD")) {

                currentConfigurationProfile_ = new ConfigurationProfile(att.getValue("name"), ConfigurationProfile.METHOD);

            }

            if (localName.equals("METHODPARAM")) {
                checkNumberFormat(att.getValue("value"));
                currentConfigurationProfile_.addParam(att.getValue("name"), att.getValue("value"));
            }



            if (localName.equals("PHASEPOINT")) {
                tempVector_ = new RealVector((new Integer(att.getValue(0))).intValue());
            }


        }

        public void endElement(String uri, String localName, String qName) throws SAXException {
            if (localName.equals("PHYSICS")) {
                RPnConfig.setActivePhysics(physicsProfile_.getName());
                RPnConfig.addProfile(physicsProfile_.getName(), physicsProfile_);
                rpnumerics.RPNUMERICS.init(physicsProfile_.getName());
                RPnConfig.createParamsFluxSubject(physicsProfile_.getName());
            }



            if (localName.equals("PHYSICSCONFIG")) {

                physicsProfile_.addConfigurationProfile(innerPhysicsConfigurationProfile_.getName(), innerPhysicsConfigurationProfile_);

            }


            if (localName.equals("CURVE") || localName.equals("METHOD")) {

                RPnConfig.addProfile(currentConfigurationProfile_.getName(), currentConfigurationProfile_);

            }

        }

        @Override
        public void characters(char[] buff, int offset, int len) throws
                SAXException {

            String data = new String(buff, offset, len);
            data = data.trim();
            if (data.length() != 0) {
                if (currentElement_.equals("REALVECTOR")) {
                    tempVector_ = new RealVector(data);
                }

            }

        }

        public void setDocumentLocator(Locator locator) {
        }

        public void startDocument() throws SAXException {
        }

        public void endDocument() throws SAXException {
        }

        public void startPrefixMapping(String prefix, String uri) throws SAXException {
        }

        public void endPrefixMapping(String prefix) throws SAXException {
        }

        public void ignorableWhitespace(char[] ch, int start, int length) throws SAXException {
        }

        public void processingInstruction(String target, String data) throws SAXException {
        }

        public void skippedEntity(String name) throws SAXException {
        }

        private void checkNumberFormat(String value) {
            try {
                Double test = new Double(value);
            } catch (NumberFormatException ex) {
                System.err.println("Invalid number format in default values file: " + value);
                System.exit(1);
            }
        }
    }

    //
    // Members
    //
    //
    // Initializers
    //
    public static void init(XMLReader parser, String file) {
        try {
            parser.setContentHandler(new InputHandler());
            parser.parse(file);
        } catch (Exception saxex) {
            saxex.printStackTrace();
        }

    }

    public static void init(XMLReader parser, InputStream configFileStream) {

        try {

            parser.setContentHandler(new InputHandler());
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
    /** Writes the actual values of the numerics parameters into a XML file. */
    public static void export(FileWriter writer) throws java.io.IOException {
        writer.write(RPNUMERICS.toXML());
    }
}
