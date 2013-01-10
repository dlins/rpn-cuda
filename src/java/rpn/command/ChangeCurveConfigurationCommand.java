/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.command;

import java.beans.PropertyChangeEvent;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;
import rpnumerics.Configuration;

public class ChangeCurveConfigurationCommand extends RpModelConfigChangeCommand {
    //
    // Constants
    //

     private static String DESC_TEXT = "Change Curve Configuration Command";
    //
    // Members
    //
    static private ChangeCurveConfigurationCommand instance_ = null;


    //
    // Constructors
    //
    protected ChangeCurveConfigurationCommand() {
        super(DESC_TEXT);
    }

    @Override
    public void applyChange(PropertyChangeEvent event) {

        Configuration newConfiguration = (Configuration) event.getNewValue();
        
        UIController.instance().logCommand(new RpCommand(new UI_ACTION_SELECTED(this), newConfiguration));


    }

    public void execute() {
    }

    public void unexecute() {
    }
    
    
    static public ChangeCurveConfigurationCommand instance() {
        if (instance_ == null) {
            instance_ = new ChangeCurveConfigurationCommand();
        }
        return instance_;
    }
}
