/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.component;

import wave.multid.view.*;
import wave.multid.*;

public class HugoniotCurveGeom extends SegmentedCurveGeom {//implements MultiGeometry, RpGeometry {
   

    //
    // Constructors
    //
    public HugoniotCurveGeom(HugoniotSegGeom[] segArray, HugoniotCurveGeomFactory factory) {

        super(segArray, factory);
//        hugoniotSegList_ = new ArrayList();
//        for (int i = 0; i < segArray.length; i++)
//            hugoniotSegList_.add(segArray[i]);
//        factory_ = factory;
//        space_ = rpnumerics.RPNUMERICS.domain();
//        try {
//            boundary_ = new BoundingBox(new CoordsArray(space_), new CoordsArray(space_));
//        } catch (DimMismatchEx dex) { dex.printStackTrace(); }
    }

//    public HugoniotCurveGeom(HugoniotCurve  hugoniotCurve, HugoniotCurveGeomFactory factory) {
//      hugoniotSegList_=hugoniotCurve.segments();
//      factory_ = factory;
//      space_ = rpnumerics.RPNUMERICS.domain();
//      try {
//        boundary_ = new BoundingBox(new CoordsArray(space_), new CoordsArray(space_));
//      } catch (DimMismatchEx dex) { dex.printStackTrace(); }
//    }
    //
    // Accessors/Mutators
    //
//    public RpGeomFactory geomFactory() { return factory_; }
//    public AbstractPathIterator getPathIterator() {
//        AbstractPath nullPath = new AbstractPath(getSpace());
//        return nullPath.getPathIterator();
//    }
//    public AbstractPathIterator getPathIterator(Map map) throws DimMismatchEx {
//        AbstractPath nullPath = new AbstractPath(getSpace());
//        return nullPath.getPathIterator(map);
//    }
//    public ViewingAttr viewingAttr() { return VIEWING_ATTR; }
//    public Iterator getHugoniotSegIterator() { return hugoniotSegList_.iterator(); }
//    public BoundingBox getBoundary() { return boundary_; }
//    public Space getSpace() { return space_; }
    //
    // Methods
    //
//    public void applyMap(Map map) {
//    }
//    public void print(FileWriter cout) { }
//    public void load(FileReader cin) { }
    public GeomObjView createView(ViewingTransform transf) throws DimMismatchEx {
        return new HugoniotCurveView(this, transf, viewingAttr());
    }

   


}
