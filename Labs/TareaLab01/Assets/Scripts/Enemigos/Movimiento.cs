using UnityEngine;

public class Movimiento : MonoBehaviour
{
    public float velocidad;
    public Rigidbody rb;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        this.transform.Translate(0.1f*velocidad,0,0);
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Muro")
        {
            velocidad *= -1;
        }      
    }
}
