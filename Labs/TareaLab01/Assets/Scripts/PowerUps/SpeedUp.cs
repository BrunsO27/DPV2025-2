using System.Collections;
using UnityEngine;

public class SpeedUp : MonoBehaviour
{
    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.tag == "Player") {
            StartCoroutine(superVel(other.gameObject));
        }
    }

    IEnumerator superVel(GameObject player) {
        this.gameObject.GetComponent<BoxCollider>().enabled = false;
        this.gameObject.GetComponent<MeshRenderer>().enabled = false;
        player.GetComponent<MovimientoPlayer>().setVelocidad(10.0f);
        yield return new WaitForSeconds(3.0f);
        player.GetComponent<MovimientoPlayer>().resetVelocidad();

        Destroy(this.gameObject);
    }
}