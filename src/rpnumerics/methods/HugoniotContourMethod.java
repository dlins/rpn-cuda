/*
 *
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics.methods;

import rpn.component.MultidAdapter;
import rpnumerics.HugoniotCurve;
import rpnumerics.HugoniotParams;
import rpnumerics.RPNUMERICS;
import rpnumerics.RPnCurve;
import rpnumerics.methods.contour.ContourHugoniot;
import rpnumerics.methods.contour.functionsobjects.CanNotPerformCalculations;
import wave.util.Boundary;
import wave.util.PointNDimension;
import wave.util.RealVector;
import wave.util.RectBoundary;

public class HugoniotContourMethod extends HugoniotMethod {

    private ContourHugoniot contourMethod_;
    private HugoniotParams hugoniotParams_;
    private double[] boundaryArray_;
    private int[] resolution_;

    public HugoniotContourMethod(HugoniotParams hugoniotParams) {

        contourMethod_ = ContourFactory.createContourHugoniot(hugoniotParams);
        resolution_ = RPNUMERICS.getContourResolution();

        Boundary boundary = RPNUMERICS.boundary();

        if (boundary instanceof RectBoundary) {

            boundaryArray_ = new double[4];

            RealVector minimums = boundary.getMinimums();
            RealVector maximums = boundary.getMaximums();

            double[] minimumsArray = minimums.toDouble();
            double[] maximumsArray = maximums.toDouble();

            boundaryArray_[0] = minimumsArray[0];
            boundaryArray_[1] = maximumsArray[0];
            boundaryArray_[2] = minimumsArray[1];
            boundaryArray_[3] = maximumsArray[1];
        } else {

            System.out.println("Implementar para dominio triangular");

        }

        hugoniotParams_ = hugoniotParams;
    }

    public HugoniotCurve curve(RealVector initialPoint) {

        HugoniotCurve hugoniotCurve = null;
        try {
            contourMethod_.setInitialPoint(new PointNDimension(RPNUMERICS.getShockProfile().getXZero()));
            RPnCurve curve = contourMethod_.curvND(boundaryArray_, resolution_);

            hugoniotCurve = new HugoniotCurve(hugoniotParams_.getXZero(), MultidAdapter.converseRPnCurveToCoordsArray(curve));

        } catch (CanNotPerformCalculations ex) {
            ex.printStackTrace();
        }

        return hugoniotCurve;

    }

    public HugoniotParams getParams() {
        return hugoniotParams_;

    }
}
