/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */

package rpn.controller.ui;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Path2D;
import rpn.RPnPhaseSpacePanel;
import wave.multid.Coords2D;
import wave.multid.CoordsArray;
import wave.multid.Space;
import wave.multid.view.ViewingTransform;
import rpnumerics.RPNUMERICS;

public abstract class RPn2DMouseController implements  MouseMotionListener, MouseListener{


    // ------ implementacao inicial
//    protected Path2D.Double plotWCArea(Point cursorPos_, MouseEvent me, RPnPhaseSpacePanel panel) {
//        ViewingTransform viewingTransform = panel.scene().getViewingTransform();
//
//        double[] cursorPosArray = {cursorPos_.x, cursorPos_.y};
//        double[] mePosArray = {me.getX(), me.getY()};
//
//        //CoordsArray cursorPosWC = new CoordsArray(new Space(" ", 2));
//        //CoordsArray mePosWC = new CoordsArray(new Space(" ", 2));
//
//        CoordsArray cursorPosWC = new CoordsArray(new Space(" ", RPNUMERICS.domainDim()));
//        CoordsArray mePosWC = new CoordsArray(new Space(" ", RPNUMERICS.domainDim()));
//
//        Coords2D cursorPosDC = new Coords2D(cursorPosArray);
//        Coords2D mePosDC = new Coords2D(mePosArray);
//
//        viewingTransform.dcInverseTransform(cursorPosDC, cursorPosWC);
//        viewingTransform.dcInverseTransform(mePosDC, mePosWC);
//
//        Path2D.Double selectionPath = new Path2D.Double();
//
//        selectionPath.moveTo(cursorPosWC.getElement(0), cursorPosWC.getElement(1));
//
//        selectionPath.lineTo(mePosWC.getElement(0), cursorPosWC.getElement(1));
//
//        selectionPath.lineTo(mePosWC.getElement(0), mePosWC.getElement(1));
//
//        selectionPath.lineTo(cursorPosWC.getElement(0), mePosWC.getElement(1));
//
//        selectionPath.closePath();
//        return selectionPath;
//
//    }
    // ------


    // ------ alteracoes do Edson
    protected Path2D.Double plotWCArea(Point cursorPos_, MouseEvent me, RPnPhaseSpacePanel panel) {
        ViewingTransform viewingTransform = panel.scene().getViewingTransform();

        double[] cursorPosArray = {cursorPos_.x, cursorPos_.y};
        double[] mePosArray = {me.getX(), me.getY()};
        int dimension = panel.scene().getViewingTransform().projectionMap().getDomain().getDim();
        CoordsArray cursorPosWC = new CoordsArray(new Space(" ", dimension));
        CoordsArray mePosWC = new CoordsArray(new Space(" ", dimension));

        int[] compIndex = panel.scene().getViewingTransform().projectionMap().getCompIndexes();

        int biggestIndex = Math.max(compIndex[0], compIndex[1]);
        int smallestIndex = Math.min(compIndex[0], compIndex[1]);

        System.out.println("smallestIndex , biggestIndex ::::: " +smallestIndex +" , " +biggestIndex);

        Coords2D cursorPosDC = new Coords2D(cursorPosArray);
        Coords2D mePosDC = new Coords2D(mePosArray);

        viewingTransform.dcInverseTransform(cursorPosDC, cursorPosWC);

        CoordsArray tempCoords = new CoordsArray(cursorPosWC);

        cursorPosWC.setElement(0, tempCoords.getElement(smallestIndex));

        cursorPosWC.setElement(1, tempCoords.getElement(biggestIndex));

        viewingTransform.dcInverseTransform(mePosDC, mePosWC);

        CoordsArray tempCoords2 = new CoordsArray(mePosWC);

        mePosWC.setElement(0, tempCoords2.getElement(smallestIndex));
        mePosWC.setElement(1, tempCoords2.getElement(biggestIndex));

        System.out.println("mePosWC ::::::::::::::::::::::::::::::::::::::: " +mePosWC.getElement(0) +" , " +mePosWC.getElement(1));

        Path2D.Double selectionPath = new Path2D.Double();

        selectionPath.moveTo(cursorPosWC.getElement(0), cursorPosWC.getElement(1));

        selectionPath.lineTo(mePosWC.getElement(0), cursorPosWC.getElement(1));

        selectionPath.lineTo(mePosWC.getElement(0), mePosWC.getElement(1));

        selectionPath.lineTo(cursorPosWC.getElement(0), mePosWC.getElement(1));

        selectionPath.closePath();
        return selectionPath;


    }
    // ------


    // ----- Testar com outra assinatura
    protected Path2D.Double plotWCArea(Point cursorPos_, Point me, RPnPhaseSpacePanel panel) {
        System.out.println("plotWCArea com nova assinatura");

        ViewingTransform viewingTransform = panel.scene().getViewingTransform();

        double[] cursorPosArray = {cursorPos_.x, cursorPos_.y};
        double[] mePosArray = {me.getX(), me.getY()};

        CoordsArray cursorPosWC = new CoordsArray(new Space(" ", RPNUMERICS.domainDim()));
        CoordsArray mePosWC = new CoordsArray(new Space(" ", RPNUMERICS.domainDim()));

        Coords2D cursorPosDC = new Coords2D(cursorPosArray);
        Coords2D mePosDC = new Coords2D(mePosArray);

        viewingTransform.dcInverseTransform(cursorPosDC, cursorPosWC);
        viewingTransform.dcInverseTransform(mePosDC, mePosWC);

        Path2D.Double selectionPath = new Path2D.Double();

        System.out.println("Coordenadas do mundo encontradas : ");
        System.out.println("cursorPosWC ::: " +cursorPosWC.getElement(0) +" , " +cursorPosWC.getElement(1));
        System.out.println("mePosWC     ::: " +mePosWC.getElement(0) +" , " +mePosWC.getElement(1));

        selectionPath.moveTo(cursorPosWC.getElement(0), cursorPosWC.getElement(1));

        selectionPath.lineTo(mePosWC.getElement(0), cursorPosWC.getElement(1));

        selectionPath.lineTo(mePosWC.getElement(0), mePosWC.getElement(1));

        selectionPath.lineTo(cursorPosWC.getElement(0), mePosWC.getElement(1));

        selectionPath.closePath();
        return selectionPath;

    }
    // -----


}
