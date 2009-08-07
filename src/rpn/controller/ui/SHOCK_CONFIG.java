/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */

package rpn.controller.ui;

import rpn.plugininterface.PluginTableModel;
import rpn.usecase.HugoniotPlotAgent;
import rpnumerics.PhasePoint;
import rpnumerics.RPNUMERICS;
import wave.util.RealVector;
import rpn.usecase.ChangeXZeroAgent;

public class SHOCK_CONFIG extends UI_ACTION_SELECTED {

    //
    // Constructors
    //
    public SHOCK_CONFIG() {
        super(HugoniotPlotAgent.instance());
        


    }

    //
    // Methods
    //
    @Override
    public void userInputComplete(rpn.controller.ui.UIController ui,
                                  RealVector userInput) {
        
        RPNUMERICS.getShockProfile().setXZero(new PhasePoint(userInput));

        super.userInputComplete(ui, userInput);

        ChangeXZeroAgent.instance().setEnabled(true);

        ui.setState(new SIGMA_CONFIG());
    }


}
