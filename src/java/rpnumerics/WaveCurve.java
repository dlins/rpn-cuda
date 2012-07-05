/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import rpn.component.MultidAdapter;
import wave.util.RealSegment;
import wave.util.RealVector;

public class WaveCurve extends RPnCurve implements WaveCurveBranch, RpSolution {
//public class WaveCurve implements WaveCurveBranch, RpSolution {

    private int[] curveTypes_;
    private static int[] curvesIndex_;
    private int family_;
    private int direction_;
    private List<WaveCurveBranch> branchList_;
    
    private double ALFA;


    public WaveCurve(int family, int increase) {
        //nao esquecer de que tem uma chamada implicita para super() aqui
        System.out.println("Chamando wave curve init");
        family_ = family;
        direction_ = increase;
        branchList_ = new ArrayList<WaveCurveBranch>();

    }


    public void add(WaveCurveBranch branch) {
        branchList_.add(branch);

    }

    public void remove(WaveCurveBranch branch) {
        branchList_.remove(branch);
    }

  


    public int[] getCurveTypes() {
        return curveTypes_;
    }

  

    public static int[] getCurvesIndex() {
        return curvesIndex_;
    }

    public int getFamily() {
        return family_;
    }

    public int getDirection() {
        return direction_;
    }

  

    public List<WaveCurveOrbit> getSubCurvesList() {

        List<WaveCurveOrbit> result= new ArrayList<WaveCurveOrbit>();

        
        for (WaveCurveBranch branch : branchList_) {

            result.addAll(branch.getSubCurvesList());

        }

        return result;
    }

    

    public List<RealSegment> segments() {

        List temp = new ArrayList();

        for (int i = 0; i < getSubCurvesList().size(); i++) {
            for (int j = 0; j < getSubCurvesList().get(i).segments().size(); j++) {
                temp.add(getSubCurvesList().get(i).segments().get(j));
            }
        }

        return temp;
        
    }


    
}
