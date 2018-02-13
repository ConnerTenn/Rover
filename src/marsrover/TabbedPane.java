/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package marsrover;


import java.awt.GridLayout;
import javax.swing.JTabbedPane;
import java.awt.event.KeyEvent;
import javax.swing.JPanel;


/**
 *
 * @author Bradley
 */
public class TabbedPane extends JTabbedPane{
    private JPanel clawPanel, videoPanel, lidarPanel;

    
    public TabbedPane(){
        super();
        
        clawPanel = new JPanel();
        videoPanel = new JPanel();
        lidarPanel = new JPanel();
        
        clawPanel.setLayout(new GridLayout());
        addTab("Claw", null, clawPanel, "Claw stuff");
        setMnemonicAt(0, KeyEvent.VK_1);
        addTab("Video Feed", null, videoPanel, "Video stuff");
        setMnemonicAt(1, KeyEvent.VK_2);
        addTab("Lidar", null, lidarPanel, "Lidar stuff");
        setMnemonicAt(2, KeyEvent.VK_3);
        
    }

    public JPanel getClawPanel() {
        return clawPanel;
    }

    public void setClawPanel(JPanel clawPanel) {
        this.clawPanel = clawPanel;
    }

    public JPanel getVideoPanel() {
        return videoPanel;
    }

    public void setVideoPanel(JPanel videoPanel) {
        this.videoPanel = videoPanel;
    }

    public JPanel getLidarPanel() {
        return lidarPanel;
    }

    public void setLidarPanel(JPanel lidarPanel) {
        this.lidarPanel = lidarPanel;
    }
   
}
