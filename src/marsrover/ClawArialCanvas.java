/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package marsrover;

import java.awt.Color;
import java.awt.Graphics;

/**
 *
 * @author Bradley
 */
public class ClawArialCanvas extends ClawCanvas{
    public ClawArialCanvas(){
        super();
    }
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        this.setBackground(Color.WHITE);
        g.setColor(Color.BLUE);
        g.drawOval(100,100,100,100);   
     }
}
