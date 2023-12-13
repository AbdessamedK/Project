package Modele;

import javax.sound.sampled.* ;
import java.io.IOException ;

public class Audio {
    private Clip in_game, line_clear, in_menu ;
    
    public Audio() {
        try {
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(getClass().getResource("../data/audio_in_game.wav"));
            in_game = AudioSystem.getClip();
            in_game.open(audioInputStream); 
            
            audioInputStream = AudioSystem.getAudioInputStream(getClass().getResource("../data/audio_line_clear.wav"));
            line_clear = AudioSystem.getClip();
            line_clear.open(audioInputStream);
            
            audioInputStream = AudioSystem.getAudioInputStream(getClass().getResource("../data/audio_in_menu.wav"));
            in_menu = AudioSystem.getClip();
            in_menu.open(audioInputStream);
            
        } catch (UnsupportedAudioFileException | IOException | LineUnavailableException e) {
            e.printStackTrace();
        }
    }
    
    private void addLineListener(Clip clip) {
        clip.addLineListener(event -> {
            if (event.getType() == LineEvent.Type.STOP) {
                if (clip.getFramePosition() == clip.getFrameLength()) {
                    clip.setFramePosition(0);
                }
            }
        });
    }
    
    public void playAudioGame() {
        if (in_game != null) {
            addLineListener(in_game) ;
            in_game.start();
        }
    }
    
    public void stopAudioGame() {
        if (in_game != null && in_game.isRunning()) {
            in_game.stop();
        }
    }
    
    public void playAudioMenu() {
        if (in_menu != null) {
            addLineListener(in_menu) ;
            in_menu.start();
        }
    }
    
    public void stopAudioMenu() {
        if (in_menu != null && in_menu.isRunning()) {
            in_menu.stop();
        }
    }
    
    public void playAudioLine() {
        if (line_clear != null) {
            line_clear.setFramePosition(0);
            line_clear.start();
        }
    }
    
    public void stopAudioLine() {
        if (line_clear != null && line_clear.isRunning()) {
            line_clear.stop();
        }
    }
    
    private boolean isEndOfAudio(Clip clip) {
        return clip != null && clip.getFramePosition() >= clip.getFrameLength();
    }
    
    public boolean isGameAudioEnd() {
        return in_game != null && in_game.getFramePosition()==0 ;
    }
    
    public boolean isMenuAudioEnd() {
        return in_menu != null && in_menu.getFramePosition()==0 ;
    }
}
