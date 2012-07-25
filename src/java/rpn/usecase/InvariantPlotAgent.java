/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package rpn.usecase;

import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.JButton;
import javax.swing.JToggleButton;
import rpn.component.*;
import rpn.controller.phasespace.InvariantsReadyImpl;
import rpn.controller.phasespace.NumConfigImpl;
import rpn.controller.phasespace.NumConfigReadyImpl;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;
import rpn.parser.RPnDataModule;
import rpnumerics.*;
import wave.util.RealVector;

/**
 *
 * @author moreira
 */
public class InvariantPlotAgent extends RpModelPlotAgent {
    //
    // Constants
    //

    static public final String DESC_TEXT = "Invariants";
    //
    // Members
    //
    static private InvariantPlotAgent instance_ = null;

    //
    // Constructors/Initializers
    //
    protected InvariantPlotAgent() {
        super(DESC_TEXT, rpn.RPnConfig.ORBIT_FWD, new JButton());
    }

    @Override
    public void actionPerformed(ActionEvent event) {

        UI_ACTION_SELECTED action = new UI_ACTION_SELECTED(this);
        action.userInputComplete(UIController.instance());// No input needed

    }

    public RpGeometry createRpGeometry(RealVector[] input) {
        throw new UnsupportedOperationException("Not supported yet.");
    }



    @Override
    public void execute() {

        List<StationaryPointGeom> listGeom = new ArrayList<StationaryPointGeom>();

        NumConfigReadyImpl state = (NumConfigReadyImpl) RPnDataModule.PHASESPACE.state();
        
        HugoniotCurveGeom hGeom = state.hugoniotGeom();
        XZeroGeom xZero = state.xzeroGeom();

        RPnDataModule.PHASESPACE.changeState(new InvariantsReadyImpl(hGeom, xZero));

        Iterator<RpGeometry> geomList = RPnDataModule.PHASESPACE.getGeomObjIterator();

        while (geomList.hasNext()) {
            RpGeometry geom = (RpGeometry) geomList.next();

            if(geom instanceof StationaryPointGeom)
                listGeom.add((StationaryPointGeom) geom);

        }

        for (StationaryPointGeom stationaryPointGeom : listGeom) {
            RPnDataModule.PHASESPACE.plot(stationaryPointGeom);
        }


    }


    static public InvariantPlotAgent instance() {
        if (instance_ == null) {
            instance_ = new InvariantPlotAgent();
        }
        return instance_;
    }

}
