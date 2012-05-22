package rpn.component;

import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.geom.GeneralPath;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import wave.multid.model.MultiGeometryImpl;
import wave.multid.view.ViewingTransform;
import wave.multid.DimMismatchEx;
import wave.multid.view.GeomObjView;
import wave.multid.view.ViewingAttr;

public class WaveCurveView extends WaveCurveOrbitGeomView {

    public WaveCurveView(MultiGeometryImpl geom, ViewingTransform transf,
            ViewingAttr attr) throws DimMismatchEx {
        super(geom, transf, attr);
    }

    @Override
    public Shape createShape() {

        GeneralPath composite = new GeneralPath(GeneralPath.WIND_EVEN_ODD);

        List<WaveCurveOrbitGeom> orbitGeomList = ((WaveCurveGeom) getAbstractGeom()).getOrbitsGeomList();

        for (WaveCurveOrbitGeom orbitGeom : orbitGeomList) {
            try {
                GeomObjView orbitView = orbitGeom.createView(getViewingTransform());

                System.out.println("Visual: " + orbitView.getClass().getCanonicalName());

//                WaveCurveOrbitGeomView orbitGeomView = (WaveCurveOrbitGeomView) orbitView;


                if (orbitView instanceof CompositeOrbitView) {
                    CompositeOrbitView teste = (CompositeOrbitView) orbitView;
                    composite.append(teste.createShape(), false);
                }

                if (orbitView instanceof ShockCurveGeomView) {
                    ShockCurveGeomView teste = (ShockCurveGeomView) orbitView;
                    composite.append(teste.createShape(), false);
                }


                if (orbitView instanceof RarefactionOrbitView) {
                    RarefactionOrbitView teste = (RarefactionOrbitView) orbitView;

                    composite.append(teste.createShape(), false);

                }







            } //        super.draw(g);
            catch (DimMismatchEx ex) {
                Logger.getLogger(WaveCurveView.class.getName()).log(Level.SEVERE, null, ex);
            }



        }








//        composite.append(super.createShape(), false);

        return composite;
    }

    @Override
    public void draw(Graphics2D g) {
//
//        List<WaveCurveOrbitGeom> orbitGeomList = ((WaveCurveGeom) getAbstractGeom()).getOrbitsGeomList();
//
//
//
//        System.out.println("tamanho da lista: " + orbitGeomList.size());
//
//        for (WaveCurveOrbitGeom orbitGeom : orbitGeomList) {
//            try {
//                System.out.println(orbitGeom.getClass().getCanonicalName());
//                WaveCurveOrbitGeomView orbitView = (WaveCurveOrbitGeomView) orbitGeom.createView(getViewingTransform());
//
//                if (orbitView instanceof CompositeOrbitView) {
//                    CompositeOrbitView teste = (CompositeOrbitView) orbitView;
//                    teste.draw(g);
//                }
//
//                if (orbitView instanceof ShockCurveGeomView) {
//                    ShockCurveGeomView teste = (ShockCurveGeomView) orbitView;
//                    teste.draw(g);
//                }
//
//
//                if (orbitView instanceof RarefactionOrbitView) {
//                    RarefactionOrbitView teste = (RarefactionOrbitView) orbitView;
//                    teste.draw(g);
//                }
//
//
////                g.setColor(orbitView.getViewingAttr().getColor());
////
////                orbitView.draw(g);



//          } //
        super.draw(g);
//            catch (DimMismatchEx ex) {
//                Logger.getLogger(WaveCurveView.class.getName()).log(Level.SEVERE, null, ex);
//            }



//        }








//        super.draw(g);

    }
}
