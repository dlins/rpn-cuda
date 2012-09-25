/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.usecase;

import java.awt.event.ActionEvent;
import java.util.Iterator;
import java.util.Observable;
import java.util.Observer;
import javax.swing.JToggleButton;
import rpn.RPnPhaseSpacePanel;
import rpn.component.RpGeometry;
import rpn.controller.ui.RPnSelectionPlotter;
import rpn.controller.ui.AREASELECTION_CONFIG;
import rpn.controller.ui.UIController;
import wave.util.RealVector;

public class AreaSelectionAgent extends RpModelPlotAgent implements Observer {

    static public final String DESC_TEXT = "Area Selection";
    static private AreaSelectionAgent instance_ = null;
    

    private AreaSelectionAgent() {
        super(DESC_TEXT, null, new JToggleButton());
    }

    @Override
    public void actionPerformed(ActionEvent event) {

        UIController.instance().setState(new AREASELECTION_CONFIG());


        Iterator<RPnPhaseSpacePanel> iterator = UIController.instance().getInstalledPanelsIterator();

        while (iterator.hasNext()) {
            RPnPhaseSpacePanel button = iterator.next();

            RPnSelectionPlotter boxPlotter = new RPnSelectionPlotter();
            button.addMouseListener(boxPlotter);
            button.addMouseMotionListener(boxPlotter);


        }


    }

    public static AreaSelectionAgent instance() {
        if (instance_ == null) {
            instance_ = new AreaSelectionAgent();
        }
        return instance_;
    }

    @Override
    public void unexecute() {
    }

    

    @Override
    public void execute() {
    }

    

    @Override
    public RpGeometry createRpGeometry(RealVector[] coords) {
        return null;
    }

    @Override
    public void update(Observable o, Object arg) {

       

    }

  
}
