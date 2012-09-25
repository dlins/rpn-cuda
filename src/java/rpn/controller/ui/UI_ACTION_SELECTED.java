/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.controller.ui;

import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import wave.util.RealVector;
import rpn.usecase.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import rpn.RPnPhaseSpacePanel;
import rpn.controller.SelectionPlotter;

public class UI_ACTION_SELECTED implements UserInputHandler {
    //
    // Members
    //

    private RpModelActionAgent actionSelected_;
    private List userInputList_;

    public UI_ACTION_SELECTED(RpModelActionAgent action) {

        actionSelected_ = action;
        userInputList_ = new ArrayList();

        if (action instanceof RpModelPlotAgent) {
            Iterator<RPnPhaseSpacePanel> iterator = UIController.instance().getInstalledPanelsIterator();
            while (iterator.hasNext()) {
                RPnPhaseSpacePanel panel = iterator.next();
              
                
                MouseMotionListener[] mouseMotionArray = (MouseMotionListener[]) panel.getListeners(MouseMotionListener.class);
                MouseListener[] mouseListenerArray = (MouseListener[]) panel.getListeners(MouseListener.class);

                for (MouseListener mouseListener : mouseListenerArray) {

                    if (mouseListener instanceof SelectionPlotter) {
                        panel.removeMouseListener(mouseListener);

                    }
                }

                for (MouseMotionListener mouseMotionListener : mouseMotionArray) {

                    if (mouseMotionListener instanceof SelectionPlotter) {
                        panel.removeMouseMotionListener(mouseMotionListener);
                    }

                }

            }

        }



    }

    public RealVector[] userInputList(rpn.controller.ui.UIController ui) {

        return UIController.inputConvertion(userInputList_);
    }

    public void userInputComplete(rpn.controller.ui.UIController ui,
            RealVector userInput) {

        userInputList_.add(new RealVector(userInput));

        if (actionSelected_ instanceof PoincareSectionPlotAgent) {
            if (isPoincareInputReady()) {
                ArrayList<RealVector> tempInputList = new ArrayList<RealVector>();
                for (RealVector inputElement : userInputList(ui)) {
                    tempInputList.add(inputElement);
                }

                UIController.instance().addCommand(new Command(this, tempInputList));

                //************************ acrescentei para testar (Leandro)
                UIController.instance().setWaitCursor();
                actionSelected_.execute();
                UIController.instance().resetCursor();
                userInputList_.clear();
                ui.panelsBufferClear();
                rpn.parser.RPnDataModule.PHASESPACE.unselectAll();
                //************************************************

            }
        } else if (UIController.instance().getState() instanceof AREASELECTION_CONFIG) {
            UIController.instance().addCommand(new Command(this, userInput));
            UIController.instance().setWaitCursor();
            actionSelected_.execute();
            UIController.instance().resetCursor();
            userInputList_.clear();
            ui.panelsBufferClear();
            rpn.parser.RPnDataModule.PHASESPACE.unselectAll();
        } else {
            UIController.instance().addCommand(new Command(this, userInput));
            UIController.instance().setWaitCursor();
            actionSelected_.execute();
            UIController.instance().resetCursor();
            userInputList_.clear();
            ui.panelsBufferClear();
            rpn.parser.RPnDataModule.PHASESPACE.unselectAll();
        }


    }

    public void userInputComplete(UIController ui) {

        UIController.instance().addCommand(new Command(this));
        UIController.instance().setWaitCursor();
        actionSelected_.execute();
        UIController.instance().resetCursor();
        userInputList_.clear();
        ui.panelsBufferClear();
        rpn.parser.RPnDataModule.PHASESPACE.unselectAll();

    }

    protected boolean isPoincareInputReady() {

        if (userInputList_.size() == rpnumerics.RPNUMERICS.domainDim()) {
            System.out.println("Return true in isPoincareInputReady() !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            return true;
        }
        return false;
    }

    protected boolean isDiagonalSelection() {

        if (userInputList_.size() == 2) {
            return true;
        }
        return false;
    }

    public RpModelActionAgent getAction() {
        return actionSelected_;
    }

    public int actionDimension() {


        return rpnumerics.RPNUMERICS.domainDim();
    }
}
