using UnityEngine;

public class Bala : MonoBehaviour
{

    public float velocidad;
    public float tiempoVida = 2f;
    public Rigidbody rb;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        rb.AddForce(transform.forward * velocidad);
        Destroy(this.gameObject, tiempoVida);
    }

    void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.tag == "Muro")
        {
            Destroy(this.gameObject);
            Destroy(col.gameObject);
        }

        if (col.gameObject.tag == "Enemigo")
        {
            Destroy(this.gameObject);
            Destroy(col.gameObject);
        }

        if (col.gameObject.tag == "Escenario") {
            Destroy(this.gameObject);
        }
    }
}