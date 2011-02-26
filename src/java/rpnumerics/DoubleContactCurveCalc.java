/*
 *
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import java.util.logging.Level;
import java.util.logging.Logger;

public class DoubleContactCurveCalc extends BifurcationCurveCalc {

    //
    // Constructors/Initializers
    //
    int xResolution_;
    int yResolution_;
    int leftFamily_;
    int rightFamily_;

    public DoubleContactCurveCalc(int xResolution, int yResolution, int leftFamily, int rightFamily) {
        this.xResolution_ = xResolution;
        this.yResolution_ = yResolution;
        this.leftFamily_ = leftFamily;
        this.rightFamily_ = rightFamily;
    }

    public DoubleContactCurveCalc() {
    }

    @Override
    public RpSolution calc() {
        RpSolution result = null;

        try {
            result = (DoubleContactCurve) nativeCalc(xResolution_,yResolution_,leftFamily_,rightFamily_);
            //          if (result == null) {
            //            throw new RpException("Error in native layer");
            //        }
            return result;
        } catch (RpException ex) {
            Logger.getLogger(DoubleContactCurveCalc.class.getName()).log(Level.SEVERE, null, ex);
        }

        return result;
    }

    private native RpSolution nativeCalc(int xResolution, int yResolution, int leftFamily, int rightFamily) throws RpException;
}
