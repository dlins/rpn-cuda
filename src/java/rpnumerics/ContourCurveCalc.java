/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import java.util.List;

public abstract class ContourCurveCalc implements RpCalculation {

    private ContourParams params_;

    //
    // Constructors
    //
    public ContourCurveCalc(ContourParams params) {
        params_ = params;

    }

    public RpSolution recalc() throws RpException {
        return calc();
    }

      //
    // Accessors/Mutators
    //
    public RpSolution recalc(List<Area> area) throws RpException {
        throw new UnsupportedOperationException("Not supported yet.");
    }


    public ContourParams getParams() {
        return params_;
    }
}
