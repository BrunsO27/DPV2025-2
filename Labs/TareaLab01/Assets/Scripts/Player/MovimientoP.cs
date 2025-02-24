using System;
using UnityEngine;

public class MovimientoPlayer : MonoBehaviour
{
    public float movHorizontal;
    public float movVertical;
    public float velocidad, velocidadOriginal;
    public float gravedad;

    public Vector3 direccion;
    public CharacterController cc;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        cc = this.gameObject.GetComponent<CharacterController>();
        velocidadOriginal = velocidad;
    }

    // Update is called once per frame
    void Update()
    {
        direccion = Vector3.zero;
        direccion.y -= gravedad * Time.deltaTime;
        direccion.x = Input.GetAxisRaw("Horizontal") * velocidad;
        direccion.z = Input.GetAxisRaw("Vertical") * velocidad;

        //this.transform.position += direccion*Time.deltaTime;

        cc.Move(direccion*Time.deltaTime);
    }

    public void setVelocidad(float velocidad) {
        this.velocidad = velocidad;
    }

    public void resetVelocidad() {
        this.velocidad = velocidadOriginal;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Objetivo")
        {
            Destroy(other.gameObject);
        }
    }
}