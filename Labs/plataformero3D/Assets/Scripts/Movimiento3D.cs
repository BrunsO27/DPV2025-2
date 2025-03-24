using UnityEngine;
using TMPro;

public class Movimiento3D : MonoBehaviour
{
    public Transform boquilla;   
    public GameObject zooka;
    public GameObject bala;
    CharacterController cc;
    Animator anim;
    public Vector3 direccion;
    public float gravedad = 9.81f;
    public float rotacion;
    public float velRotacion = 5f;
    public float salto = 5f;  // Valor más razonable para el salto
    public float velocidad = 5f;
    public float vertical;
    private float velocidadVertical = 0f; // Almacena la velocidad en Y
    private bool shoot = false;
    public int numBala;
    public TMP_Text balas;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        anim = gameObject.GetComponent<Animator>();
        cc = gameObject.GetComponent<CharacterController>();  
    }

    // Update is called once per frame
    void Update()
    {
        if (cc.isGrounded)
        {
            anim.SetTrigger("suelo");
            anim.SetFloat("move", vertical);
            vertical = Input.GetAxisRaw("Vertical");
            direccion = gameObject.transform.TransformDirection(new Vector3(0, 0, vertical * velocidad));
            rotacion = Input.GetAxis("Horizontal") * velRotacion;

            if (vertical == 0)
            {
                anim.SetBool("idle", true);                
            } else
            {
                anim.SetBool("idle", false);
            }

            if (Input.GetKeyDown(KeyCode.J))
            {   
                anim.SetTrigger("jump");
                Debug.Log("Salto");
                velocidadVertical = salto; // Aplicar impulso inicial de salto
            }
        } else
        {
            anim.ResetTrigger("suelo");
        }

        // Aplicar gravedad continuamente
        velocidadVertical -= gravedad * Time.deltaTime;
        direccion.y = velocidadVertical;

        // Aplicar movimiento y rotación
        cc.transform.Rotate(new Vector3(0, rotacion, 0));
        cc.Move(direccion * Time.deltaTime);

        if (Input.GetKeyDown(KeyCode.K) && shoot)
        {
            Instantiate(bala, boquilla.position, boquilla.rotation);
            numBala++;
            balas.text = numBala.ToString();
        }
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Zooka")
        {
            zooka.SetActive(true);
            other.gameObject.SetActive(false);
            this.shoot = true;
            Debug.Log("Zooka");
        }
    }
}
