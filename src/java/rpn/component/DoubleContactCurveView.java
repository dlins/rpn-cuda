/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.component;

import java.awt.Color;
import wave.multid.model.*;
import wave.multid.view.*;
import wave.multid.DimMismatchEx;
import java.awt.Graphics2D;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;

public class DoubleContactCurveView
        implements GeomObjView {
    //
    // Members
    //

    private List viewList_;
    private ViewingTransform viewingTransf_;
    private AbstractGeomObj abstractGeom_;
    private ViewingAttr viewAttr_;

    //
    // Constructor
    //
    public DoubleContactCurveView(DoubleContactCurveGeom abstractGeom,
            ViewingTransform transf,
            ViewingAttr viewAttr) throws DimMismatchEx {
        setAbstractGeom(abstractGeom);
        setViewingTransform(transf);
        setViewingAttr(viewAttr);
        viewList_ = new ArrayList();
        update();
    }

    //
    // Accessors/Mutators
    //
    public AbstractGeomObj getAbstractGeom() {
        return abstractGeom_;
    }

    public void setAbstractGeom(AbstractGeomObj abstractGeom) {
        abstractGeom_ = abstractGeom;
    }

    public ViewingTransform getViewingTransform() {
        return viewingTransf_;
    }

    public void setViewingTransform(ViewingTransform transf) {
        viewingTransf_ = transf;
    }

    public ViewingAttr getViewingAttr() {
        return viewAttr_;
    }

    public void setViewingAttr(ViewingAttr viewAttr) {
        viewAttr_ = viewAttr;
    }

    //
    // Methods
    //
    public void draw(Graphics2D g) {

        for (int i = 0; i < viewList_.size(); i++) {
        //for (int i = 0; i < viewList_.size()/2; i++) {                 // *********  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            ((GeomObjView) viewList_.get(i)).draw(g);
        }

    }

    //Original update method
    public void update() {
        viewList_.clear();

        DoubleContactCurveGeom doubleContactGeom = (DoubleContactCurveGeom) abstractGeom_;
        Iterator geomListIterator = doubleContactGeom.getBifurcationSegmentsIterator();


        while (geomListIterator.hasNext()) {
            RealSegGeom geomObj = (RealSegGeom) geomListIterator.next();
            try {
                if (geomObj == null) {
                    System.out.println("Eh nulo segmento");
                }

                viewList_.add(geomObj.createView(getViewingTransform()));
            } catch (DimMismatchEx dex) {
                dex.printStackTrace();
            }
        }
    }
}
