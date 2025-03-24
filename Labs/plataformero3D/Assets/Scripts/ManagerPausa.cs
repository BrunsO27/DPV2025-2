using UnityEngine;
using UnityEngine.SceneManagement;

public class ManagerPausa : MonoBehaviour
{
    public GameObject menuPausa; 
    public bool isPaused;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.P)) {
            this.Pausar();
        }
    }

    public void Pausar() {
        if (!isPaused) {
            menuPausa.SetActive(true);
            Time.timeScale = 0;
            isPaused = !isPaused;
        } else {
            menuPausa.SetActive(false);
            Time.timeScale = 1;
            isPaused = !isPaused;
        }
    }

    public void IniciarJuego() {
        SceneManager.LoadScene(1);
    }

    public void RegresarMenu() {
        SceneManager.LoadScene(0);
        Time.timeScale = 1;
    }
}
