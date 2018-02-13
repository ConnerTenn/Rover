/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package marsrover;

import javax.swing.JPanel;

/**
 *
 * @author Bradley
 */
public class ClawCanvas extends JPanel{
    private Double base_angle_horizontal;
    private Double base_angle_vertical;
    private Double angleMid;
    private Double angleClaw;
     
    private int clampDistance;
    
    
    public ClawCanvas(){
        base_angle_horizontal = 0.0;
        base_angle_vertical = 0.0;
        angleClaw = 0.0;
        angleMid = 0.0;
        clampDistance = 0;            
    }
        
}
