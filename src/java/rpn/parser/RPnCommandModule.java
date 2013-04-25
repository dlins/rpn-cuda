/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.parser;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;

import rpn.command.*;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;

import rpnumerics.RPNUMERICS;


import wave.util.RealVector;

import org.xml.sax.SAXException;
import org.xml.sax.InputSource;
import org.xml.sax.ContentHandler;
import org.xml.sax.XMLReader;

import java.io.*;
import java.util.Iterator;


import rpn.configuration.Configuration;
import rpn.controller.ui.UndoActionController;
import rpn.controller.ui.UserInputHandler;

/** With this class the calculus made in a previous session can be reloaded. A previous state can be reloaded reading a XML file that is used by this class */
public class RPnCommandModule {

    static public class RPnCommandParser implements ContentHandler {

        private String currentElement_;
        private String currentCommand_;
        private StringBuilder stringBuffer_ = new StringBuilder();
        private Configuration currentConfiguration_;
        private boolean isChangePhysicsParamsCommand_;
        private Integer curveId_;

        public RPnCommandParser() {


            stringBuffer_ = new StringBuilder();
            isChangePhysicsParamsCommand_ = false;

        }

        @Override
        public void endDocument() {
        }

        @Override
        public void startElement(String uri, String name, String qName, Attributes att) throws
                SAXException {

            // TODO : eventually the COMMANDs should be in a differente file from the CONFIG
            // and the COMMAND tag would carry away the phasespace information with it !??
            currentElement_ = name;

            if (isChangePhysicsParamsCommand_) {

                if (currentElement_.equals("PHYSICSCONFIG")) {
                    System.out.println("Current configuration: " + currentConfiguration_.getName());

                    currentConfiguration_ = currentConfiguration_.getConfiguration(att.getValue("name"));

                }


                if (currentElement_.equals("PHYSICSPARAM")) {

                    currentConfiguration_.setParamValue(att.getValue("name"), att.getValue("value"));

                }

            }



            if (currentElement_.equals("CURVECONFIGURATION")) {
                currentConfiguration_ = rpnumerics.RPNUMERICS.getConfiguration(att.getValue("name"));
            }

            if (currentElement_.equals("CURVEPARAM")) {

                currentConfiguration_.setParamValue(att.getValue(0), att.getValue(1));
            }


            if (currentElement_.equals("COMMANDPARAM")) {

                for (int i = 0; i < att.getLength(); i++) {

                    System.out.println(att.getValue(i));
                }


                if (att.getValue("name").equals("phasespace")) {

                    UIController.instance().setActivePhaseSpace(RPnDataModule.getPhaseSpace(att.getValue("value")));

                }

                if (att.getValue("name").equals("curveid")) {
                    curveId_ = new Integer(att.getValue("value"));
                }
            }



            if (currentElement_.equalsIgnoreCase("COMMAND")) {
                if (att.getValue("curveid") != null) {
                    curveId_ = new Integer(att.getValue("curveid"));

                }
                currentCommand_ = att.getValue("name");

                System.out.println("Current command :" + currentCommand_);

                UIController.instance().setActivePhaseSpace(RPnDataModule.getPhaseSpace(att.getValue("phasespace")));


                if (currentCommand_.equalsIgnoreCase("Change Flux Parameters")) {
                    currentConfiguration_ = RPNUMERICS.getConfiguration(RPNUMERICS.physicsID());
                } else if (currentCommand_.equalsIgnoreCase("hugoniotcurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(HugoniotPlotCommand.instance()));

                } else if (currentCommand_.equalsIgnoreCase("integralcurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(IntegralCurvePlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("wavecurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(WaveCurvePlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("levelcurve")) {
                    LevelCurvePlotCommand.instance().execute();
                } else if (currentCommand_.equalsIgnoreCase("pointlevelcurve")) {

                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(PointLevelCurvePlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("compositecurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(CompositePlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("rarefactioncurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(RarefactionCurvePlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("rarefactionextensioncurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(RarefactionExtensionCurvePlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("shockcurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(ShockCurvePlotCommand.instance()));

                } else if (currentCommand_.equalsIgnoreCase("poincaresection")) {

                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(PoincareSectionPlotCommand.instance()));
                } else if (currentCommand_.equalsIgnoreCase("doublecontactcurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(DoubleContactCommand.instance()));
                    DoubleContactCommand.instance().execute();
                } else if (currentCommand_.equalsIgnoreCase("inflectioncurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(InflectionPlotCommand.instance()));
                    InflectionPlotCommand.instance().execute();
                } else if (currentCommand_.equalsIgnoreCase("hysteresiscurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(HysteresisPlotCommand.instance()));
                    HysteresisPlotCommand.instance().execute();
                } else if (currentCommand_.equalsIgnoreCase("boundaryextensioncurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(BoundaryExtensionCurveCommand.instance()));
                    BoundaryExtensionCurveCommand.instance().execute();

                } else if (currentCommand_.equalsIgnoreCase("secondarybifurcationcurve")) {
                    UIController.instance().setStateFromFile(new UI_ACTION_SELECTED(SecondaryBifurcationCurveCommand.instance()));
                    SecondaryBifurcationCurveCommand.instance().execute();

                }
            }



            if (currentElement_.equals("REALVECTOR")) {

                stringBuffer_ = new StringBuilder();
		stringBuffer_.append(att.getValue("coords"));
            }
        }

        @Override
        public void endElement(String uri, String name, String qName) throws SAXException {


            if (name.equals("REALVECTOR")) {

                UserInputHandler userInput = UIController.instance().getState();

                if (userInput instanceof UI_ACTION_SELECTED) {

                    UI_ACTION_SELECTED actionSelected = (UI_ACTION_SELECTED) userInput;
                    RealVector input = new RealVector(stringBuffer_.toString());
                    UIController.instance().globalInputTable().reset();

                    for (int i = 0; i < input.getSize(); i++) {

                        UIController.instance().globalInputTable().setElement(i, input.getElement(i));
                    }

                    actionSelected.userInputComplete(UIController.instance(), input, curveId_);

  
                } else {
                    UIController.instance().userInputComplete(new RealVector(stringBuffer_.toString()));
                }
                  


            }


            if (name.equals("PAUSE")) {
                try {
                    System.in.read();
                } catch (java.io.IOException ex) {
                }
            }

            if (name.equals("RPCONFIGURATION")) {
                isChangePhysicsParamsCommand_ = true;
            }


            if (name.equals("COMMAND")) {


                if (currentCommand_.equals("Curve Remove Command")) {
                    CurveRemoveCommand.instance().remove(curveId_);
                }
            }


        }

	public void characters(char[] c,int i,int j) {
	}

        public void setDocumentLocator(Locator locator) {
        }

        public void startDocument() throws SAXException {
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

        public static void selectPhaseSpace(String phaseSpaceName) {

            if (phaseSpaceName != null) {

                if (phaseSpaceName.equals("Phase Space")) {
                    UIController.instance().setActivePhaseSpace(RPnDataModule.PHASESPACE);
                } else if (phaseSpaceName.equals("LeftPhase Space")) {
                    UIController.instance().setActivePhaseSpace(RPnDataModule.LEFTPHASESPACE);
                } else if (phaseSpaceName.equals("RightPhase Space")) {
                    UIController.instance().setActivePhaseSpace(RPnDataModule.RIGHTPHASESPACE);
                }

            }
        }
    }

    //
    // Initializers
    //
    /** Initializes the XML parser to reload a previous session. */
    public static void init(XMLReader parser, String configFile) {
        try {
            parser.setContentHandler(new RPnCommandParser());
            System.out.println("Command Module parsing started...");
            parser.parse(configFile);
            System.out.println("Command Module parsing finshed sucessfully !");
        } catch (Exception saxex) {

            saxex.printStackTrace();

        }
    }

    /** Initializes the XML parser to reload a previous session. */
    public static void init(XMLReader parser, InputStream configFileStream) {
        try {
            parser.setContentHandler(new RPnCommandParser());
            System.out.println("Command Module parsing started...");
            parser.parse(new InputSource((configFileStream)));
            System.out.println("Command Module parsing finished sucessfully !");
        } catch (Exception saxex) {

            if (saxex instanceof org.xml.sax.SAXParseException) {
                System.out.println("Line: "
                        + ((org.xml.sax.SAXParseException) saxex).getLineNumber());
                System.out.println("Column: "
                        + ((org.xml.sax.SAXParseException) saxex).getColumnNumber());
            }

            saxex.printStackTrace();
        }
    }

    //
    // Methods
    //
    /** Writes the data of actual session into a XML file. */
    static public void export(FileWriter writer) throws java.io.IOException {

        System.out.println("Command module export started...");

        Iterator<RpCommand> iterator1 = UndoActionController.instance().getCommandIterator();

        while (iterator1.hasNext()) {
            writer.write(((RpCommand) iterator1.next()).toXML());
        }

        System.out.println("Command module export finished sucessfully !");
    }
}
