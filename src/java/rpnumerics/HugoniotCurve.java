
package rpnumerics;

import wave.util.RealVector;
import wave.util.RealSegment;
import java.util.List;
import java.util.ArrayList;

public class HugoniotCurve extends SegmentedCurve {
    //
    // Members
    //

    private PhasePoint xZero_;
 

    public HugoniotCurve(PhasePoint xZero, List<HugoniotSegment> hSegments) {
        super(hSegments);

        xZero_ = new PhasePoint(xZero);
    

    }

    public static List interpolate(HugoniotPoint v1,
            HugoniotPoint v2) {
        List segments = new ArrayList();
        // dimension
        int m = v1.getSize();
        int negativeRealPartNoRight1 = v1.type().negativeRealPartNoRight();
        int zeroRealPartNoRight1 = v1.type().zeroRealPartNoRight();
        int positiveRealPartNoRight1 = v1.type().positiveRealPartNoRight();
        int negativeRealPartNoLeft1 = v1.type().negativeRealPartNoLeft();
        int zeroRealPartNoLeft1 = v1.type().zeroRealPartNoLeft();
        int positiveRealPartNoLeft1 = v1.type().positiveRealPartNoLeft();
        int negativeRealPartNoRight2 = v2.type().negativeRealPartNoRight();
        int zeroRealPartNoRight2 = v2.type().zeroRealPartNoRight();
        int positiveRealPartNoRight2 = v2.type().positiveRealPartNoRight();
        int negativeRealPartNoLeft2 = v2.type().negativeRealPartNoLeft();
        int zeroRealPartNoLeft2 = v2.type().zeroRealPartNoLeft();
        int positiveRealPartNoLeft2 = v2.type().positiveRealPartNoLeft();
        // number and direction of changes in the left state
        int leftStateChangesNo = 0;
        if ((positiveRealPartNoLeft2 - positiveRealPartNoLeft1 < 0)
                && ((positiveRealPartNoLeft2 - positiveRealPartNoLeft1)
                * (positiveRealPartNoLeft2 - positiveRealPartNoLeft1
                + zeroRealPartNoLeft2) > 0)
                || (positiveRealPartNoLeft2 - positiveRealPartNoLeft1 > 0)
                && ((positiveRealPartNoLeft2 - positiveRealPartNoLeft1)
                * (positiveRealPartNoLeft2 - positiveRealPartNoLeft1
                - zeroRealPartNoLeft1) > 0)) {
            // there are changes of type
            // determine how many
            if (positiveRealPartNoLeft2 - positiveRealPartNoLeft1 > 0) {
                leftStateChangesNo = positiveRealPartNoLeft2
                        - positiveRealPartNoLeft1
                        - zeroRealPartNoLeft1;
                positiveRealPartNoLeft1 += zeroRealPartNoLeft1;
                zeroRealPartNoLeft1 = 0;
            } else {
                leftStateChangesNo = positiveRealPartNoLeft2
                        + zeroRealPartNoLeft2
                        - positiveRealPartNoLeft1;
                negativeRealPartNoLeft1 += zeroRealPartNoLeft1;
                zeroRealPartNoLeft1 = 0;
            }
        } else {
            // no change of type
            // determine left type inside segment
            positiveRealPartNoLeft1 = Math.max(positiveRealPartNoLeft1,
                    positiveRealPartNoLeft2);
            negativeRealPartNoLeft1 = Math.max(negativeRealPartNoLeft1,
                    negativeRealPartNoLeft2);
            zeroRealPartNoLeft1 = m - positiveRealPartNoLeft1
                    - negativeRealPartNoLeft1;
        }
        // number and direction of changes in the right state
        int rightStateChangesNo = 0;
        if ((positiveRealPartNoRight2 - positiveRealPartNoRight1 < 0)
                && ((positiveRealPartNoRight2 - positiveRealPartNoRight1)
                * (positiveRealPartNoRight2 - positiveRealPartNoRight1
                + zeroRealPartNoRight2) > 0)
                || (positiveRealPartNoRight2 - positiveRealPartNoRight1 > 0)
                && ((positiveRealPartNoRight2 - positiveRealPartNoRight1)
                * (positiveRealPartNoRight2 - positiveRealPartNoRight1
                - zeroRealPartNoRight1) > 0)) {
            // there are changes of type
            // determine how many
            if (positiveRealPartNoRight2 - positiveRealPartNoRight1 > 0) {
                rightStateChangesNo = positiveRealPartNoRight2
                        - positiveRealPartNoRight1
                        - zeroRealPartNoRight1;
                positiveRealPartNoRight1 += zeroRealPartNoRight1;
                zeroRealPartNoRight1 = 0;
            } else {
                rightStateChangesNo = positiveRealPartNoRight2
                        + zeroRealPartNoRight2
                        - positiveRealPartNoRight1;
                negativeRealPartNoRight1 += zeroRealPartNoRight1;
                zeroRealPartNoRight1 = 0;
            }
        } else {
            // no change of type
            // determine right type inside segment
            positiveRealPartNoRight1 = Math.max(positiveRealPartNoRight1,
                    positiveRealPartNoRight2);
            negativeRealPartNoRight1 = Math.max(negativeRealPartNoRight1,
                    negativeRealPartNoRight2);
            zeroRealPartNoRight1 = m - positiveRealPartNoRight1
                    - negativeRealPartNoRight1;
        }
        // cutting and creating segments
        HugoniotPointType type;
        double alphaLeft = 0, alphaRight = 0;
        int dLeft = 0, dRight = 0;
        double sigma1 = v1.sigma();
        RealVector x1 = new RealVector(v1);
        RealVector x2 = new RealVector(v2);
        double sigma2Left = v2.sigma(), sigma2Right = v2.sigma();
        type = new HugoniotPointType(negativeRealPartNoLeft1,
                zeroRealPartNoLeft1,
                positiveRealPartNoLeft1,
                negativeRealPartNoRight1,
                zeroRealPartNoRight1,
                positiveRealPartNoRight1);
        int count = 0;
        while ((leftStateChangesNo != 0) || (rightStateChangesNo != 0)) {
            // find nearest change of left type
            if (leftStateChangesNo > 0) {
                dLeft = 1;
                alphaLeft = v1.eigenValRLeft().getElement(
                        negativeRealPartNoLeft1 - 1)
                        / (v1.eigenValRLeft().getElement(
                        negativeRealPartNoLeft1 - 1)
                        - v2.eigenValRLeft().getElement(
                        negativeRealPartNoLeft1 - 1));
                sigma2Left = (1 - alphaLeft) * v1.sigma()
                        + alphaLeft * v2.sigma();
            }
            if (leftStateChangesNo < 0) {
                dLeft = -1;
                alphaLeft = v1.eigenValRLeft().getElement(m
                        - positiveRealPartNoLeft1)
                        / (v1.eigenValRLeft().getElement(m
                        - positiveRealPartNoLeft1)
                        - v2.eigenValRLeft().getElement(m
                        - positiveRealPartNoLeft1));
                sigma2Left = (1 - alphaLeft) * v1.sigma()
                        + alphaLeft * v2.sigma();
            }
            if (leftStateChangesNo == 0) {
                dLeft = 0;
                alphaLeft = 1;
                sigma2Left = v2.sigma();
            }
            // find nearest change of right type
            if (rightStateChangesNo > 0) {
                dRight = 1;
                alphaRight = v1.eigenValRRight().getElement(
                        negativeRealPartNoRight1 - 1)
                        / (v1.eigenValRRight().getElement(
                        negativeRealPartNoRight1 - 1)
                        - v2.eigenValRRight().getElement(
                        negativeRealPartNoRight1 - 1));
                sigma2Right = (1 - alphaRight) * v1.sigma()
                        + alphaRight * v2.sigma();
            }
            if (rightStateChangesNo < 0) {
                dRight = -1;
                alphaRight = v1.eigenValRRight().getElement(m
                        - positiveRealPartNoRight1)
                        / (v1.eigenValRRight().getElement(m
                        - positiveRealPartNoRight1)
                        - v2.eigenValRRight().getElement(m
                        - positiveRealPartNoRight1));
                sigma2Right = (1 - alphaRight) * v1.sigma()
                        + alphaRight * v2.sigma();
            }
            if (rightStateChangesNo == 0) {
                dRight = 0;
                alphaRight = 1;
                sigma2Right = v2.sigma();
            }
            // create segment to the nearest change point
            if (alphaLeft < alphaRight) {
                x2.set(v1);
                x2.interpolate(v2, alphaLeft);
                if (!(x1.equals(x2)) || (sigma1 != sigma2Left)) {
                    segments.add(new HugoniotSegment(x1, sigma1, x2, sigma2Left,
                            type));
                }
                x1.set(x2);
                sigma1 = sigma2Left;
                leftStateChangesNo -= dLeft;
                negativeRealPartNoLeft1 -= dLeft;
                positiveRealPartNoLeft1 += dLeft;
                type = new HugoniotPointType(negativeRealPartNoLeft1,
                        zeroRealPartNoLeft1,
                        positiveRealPartNoLeft1,
                        negativeRealPartNoRight1,
                        zeroRealPartNoRight1,
                        positiveRealPartNoRight1);
            } else {
                x2.set(v1);
                x2.interpolate(v2, alphaRight);
                if (!(x1.equals(x2)) || (sigma1 != sigma2Right)) {
                    segments.add(new HugoniotSegment(x1, sigma1, x2,
                            sigma2Right, type));
                }
                x1.set(x2);
                sigma1 = sigma2Right;
                rightStateChangesNo -= dRight;
                negativeRealPartNoRight1 -= dRight;
                positiveRealPartNoRight1 += dRight;
                type = new HugoniotPointType(negativeRealPartNoLeft1,
                        zeroRealPartNoLeft1,
                        positiveRealPartNoLeft1,
                        negativeRealPartNoRight1,
                        zeroRealPartNoRight1,
                        positiveRealPartNoRight1);
            }
        }
        if (!(x1.equals(v2)) || (sigma1 != sigma2Left)) {
            segments.add(new HugoniotSegment(x1, sigma1, v2, v2.sigma(), type));
        }
        return segments;
    }

    @Override
    public int findClosestSegment(RealVector targetPoint, double alpha) {

        RealVector target = new RealVector(targetPoint);
        RealVector closest = null;
        RealVector segmentVector = null;
        alpha = 0;
        int closestSegment = 0;
        double closestDistance = -1;

        List hugoniotSegList = segments();
        for (int i = 0; i < segments().size(); i++) {

            HugoniotSegment segment = (HugoniotSegment) hugoniotSegList.get(i);
            segmentVector = new RealVector(segment.rightPoint());
            segmentVector.sub(segment.leftPoint());
            closest = new RealVector(target);
            closest.sub(segment.rightPoint());
            alpha = closest.dot(segmentVector)
                    / segmentVector.dot(segmentVector);
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha > 1) {
                alpha = 1;
            }
            segmentVector.scale(alpha);
            closest.sub(segmentVector);
            if ((closestDistance < 0) || (closestDistance > closest.norm())) {
                closestSegment = i;
                closestDistance = closest.norm();
            }
        }


        return closestSegment;
    }

    private static List hugoniotSegsFromWaveState(PhasePoint xZero, WaveState[] wStates) {

        ArrayList result = new ArrayList();

        int inputSize = wStates.length;
        for (int i = 0; i < inputSize - 1; i++) {
            // type is set...
            HugoniotPoint v1 = new HugoniotPoint(xZero,
                    wStates[i].finalState().
                    getCoords(),
                    wStates[i].speed());
            HugoniotPoint v2 = new HugoniotPoint(xZero,
                    wStates[i
                    + 1].finalState().getCoords(),
                    wStates[i + 1].speed());
            if ((v1.type().equals(v2.type()))) {
                result.add(new HugoniotSegment(v1, v1.sigma(), v2, v2.sigma(),
                        v1.type()));
            } else {
                List partCurve = interpolate(v1, v2);
                result.addAll(partCurve);
            }
        }

        return result;

    }

    private static List hugoniotSegsFromWaveState(PhasePoint xZero, List wStates) {

        ArrayList result = new ArrayList();

        int inputSize = wStates.size();
        for (int i = 0; i < inputSize - 1; i++) {
            // type is set...
            HugoniotPoint v1 = new HugoniotPoint(xZero,
                    ((WaveState) wStates.get(i)).finalState().
                    getCoords(),
                    ((WaveState) wStates.get(i)).speed());
            HugoniotPoint v2 = new HugoniotPoint(xZero,
                    ((WaveState) wStates.get(i + 1)).finalState().getCoords(), ((WaveState) wStates.get(i + 1)).speed());

            if ((v1.type().equals(v2.type()))) {
                result.add(new HugoniotSegment(v1, v1.sigma(), v2, v2.sigma(),
                        v1.type()));
            } else {
                List partCurve = interpolate(v1, v2);
                result.addAll(partCurve);
            }
        }

        return result;

    }

    
    private static List hugoniotSegsFromRealSegs(PhasePoint xZero_,
            List realSegs) {

        ArrayList result = new ArrayList();

        int inputSize = realSegs.size();
        for (int i = 0; i < inputSize; i++) {
            // type is set...

            HugoniotPoint v1 = new HugoniotPoint(xZero_,
                    ((RealSegment) realSegs.get(i)).p1());
            HugoniotPoint v2 = new HugoniotPoint(xZero_,
                    ((RealSegment) realSegs.get(i)).p2());
            if ((v1.type().equals(v2.type()))) {
                result.add(new HugoniotSegment(v1, v1.sigma(), v2, v2.sigma(),
                        v1.type()));
            } else {
                List partCurve = interpolate(v1, v2);
                result.addAll(partCurve);
            }
        }
        return result;
    }

   

    public double findSigma(PhasePoint targetPoint) {

        int alpha = 0;
        int hugoniotSegmentIndx = findClosestSegment(targetPoint, alpha);

        HugoniotSegment segment = (HugoniotSegment) segments().get(
                hugoniotSegmentIndx);

        return (segment.leftSigma() * (1 - alpha)
                + segment.rightSigma() * alpha);

    }

    public List findPoints(double sigma) {
        ArrayList points = new ArrayList();
        double alpha = 0;
        RealVector point = null;

        for (int i = 0; i < segments().size(); i++) {

            HugoniotSegment segment = (HugoniotSegment) segments().get(i);

            if ((sigma - segment.leftSigma()) * (sigma - segment.rightSigma())
                    <= 0) {
                alpha = (segment.leftSigma() - sigma)
                        / (segment.leftSigma() - segment.rightSigma());
                point = new RealVector(segment.leftPoint());
                point.interpolate(segment.rightPoint(), alpha);
                points.add(point);
            }
        }
        return points;
    }



//    private String createMatlabFor(int x, int y,int identifier){
//        int dimension = RPNUMERICS.domainDim();
//        StringBuffer buffer = new StringBuffer();
//        buffer.append("xlabel('"+x+"')");
//        buffer.append("ylabel('" + x + "')");
//        buffer.append("for i=1: length(data" + identifier + ")");
//        buffer.append("plot([ data"+identifier);
//
//        buffer.append("(i"+","+x+") ");
//        buffer.append("data" + identifier + "(i," + (x+dimension) + ")],");
//
//        buffer.append("[ data"+identifier);
//
//        buffer.append("(i"+","+y+") ");
//        buffer.append("data" + identifier + "(i," + (y+dimension) + ")])");
//
//        buffer.append("hold on");
//
////        figure(1) % Assume figure(1) corresponds to the x-y projection
////xlabel('x')
////ylabel('y')
////for i = 1: length(data0)
////    plot([data0(i, 1) data0(i, 4)], [data0(i, 2) data

   
    

//    public String toMatlabData() {
//
//        StringBuffer buffer = new StringBuffer();
//
//        for (int i = 0; i < hugoniotSegments_.size(); i++) {
//
//
//            HugoniotSegment hSegment = ((HugoniotSegment) hugoniotSegments_.get(
//                    i));
//
//
//            double leftSigma = hSegment.leftSigma();
//            double rightSigma = hSegment.rightSigma();
//
//            RealSegment rSegment = new RealSegment(hSegment.leftPoint(),
//                    hSegment.rightPoint());
//
//
//
//
//            buffer.append("data" + i + "= [" + rSegment.toString() + "   " + leftSigma + " " + rightSigma + " " + hSegment.getLeftLambdaArray()[0] + " " + hSegment.getLeftLambdaArray()[1] + " " + hSegment.getRightLambdaArray()[0] + " " + hSegment.getRightLambdaArray()[1] + "];\n\n");
//
//            int type = hSegment.getType() + 1;
//            buffer.append("% type of segment: " + type + "\n");
//
//            buffer.append("plot3([data");
//            buffer.append(i);
//            buffer.append("(" + (1) + ") ");
//            buffer.append("data");
//            buffer.append(i);
//            buffer.append("(" + (4) + ")], ");
//
//            buffer.append("[data");
//            buffer.append(i);
//            buffer.append("(" + (2) + ") ");
//            buffer.append("data");
//            buffer.append(i);
//            buffer.append("(" + (5) + ")], ");
//
//            buffer.append("[data");
//            buffer.append(i);
//            buffer.append("(" + (3) + ") ");
//            buffer.append("data");
//            buffer.append(i);
//            buffer.append("(" + (6) + ")]");
//
//            buffer.append(", \'Color\', [toc(");
//            buffer.append(type);
//            buffer.append(", 1) toc(");
//            buffer.append(type);
//            buffer.append(", 2) toc(");
//            buffer.append(type);
//            buffer.append(", 3)])\n");
//            if (i == 0) {
//                RealVector xMin = RPNUMERICS.boundary().getMinimums();
//                RealVector xMax = RPNUMERICS.boundary().getMaximums();
//
//                buffer.append("axis([" + xMin.getElement(0) + " " + xMax.getElement(0) + " " + xMin.getElement(1) + " " + xMax.getElement(1) + " " + xMin.getElement(2) + " " + xMax.getElement(2) + " " + "]);\n");
//
//            }
//            if (i < hugoniotSegments_.size() - 1) {
//                buffer.append("hold on\n\n");
//            }
//        }
//
//
//        return buffer.toString();
//    }
//    public String toMatlabData() {
//
//        StringBuffer buffer = new StringBuffer();
//        for (int i = 0; i < hugoniotSegments_.size(); i++) {
//
//
//            HugoniotSegment hSegment = ((HugoniotSegment) hugoniotSegments_.get(
//                    i));
//            RealSegment rSegment = new RealSegment(hSegment.leftPoint(),
//                    hSegment.rightPoint());
//
//            buffer.append("% type of segment: " + hSegment.getType() + "\n");
//
//            buffer.append("data" + i + "= [" + rSegment.toString() + "];\n\n");
//        }
//        return buffer.toString();
//    }
    public String toXML(boolean calcReady) {
        StringBuffer buffer = new StringBuffer();
        if (calcReady) {

            buffer.append("<HUGONIOTCURVE>\n");

            for (int i = 0; i < segments().size(); i++) {

                HugoniotSegment hSegment = ((HugoniotSegment) segments().get(
                        i));
                RealSegment rSegment = new RealSegment(hSegment.leftPoint(),
                        hSegment.rightPoint());
                buffer.append(rSegment.toXML());

            }
            buffer.append("</HUGONIOTCURVE>\n");


        }

        return buffer.toString();

    }

    public PhasePoint getXZero() {
        return xZero_;
    }
}
