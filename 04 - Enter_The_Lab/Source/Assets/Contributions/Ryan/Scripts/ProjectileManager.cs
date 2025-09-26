using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileManager : MonoBehaviour
{
    private int damage;
    private List<Student> Allies;
    private Vector3 velocity;
    private bool active;

    public AudioClip blockedSound;
    public AudioClip impactSound;

    private float BlockChance;
    public float DecayRate; // By percentage per second
    public float Limit;     // Percentage of BlockChance where the projectile destroys itself automatically
	public int HunkerBlock;	// Damage blocked by hunker ability

    public void Init(int d, Vector3 p, Vector3 v, List<Student> Students)
    {
        damage = d;
        gameObject.transform.position = p;
        velocity = v;
        Allies = Students;
        active = true;
        BlockChance = 0.0f;
    }

    private void FixedUpdate()
    {
        gameObject.transform.position += velocity * Time.deltaTime;
        BlockChance += DecayRate * Time.deltaTime;

        if (BlockChance >= Limit)
        {
            FindObjectOfType<GameLogicMP>().SendStudentHit(gameObject);
            // TODO Event here...
            active = false;
            Destroy(gameObject);
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (active == true)
        {
            // If collided with a student
            if (collision.gameObject.GetComponent<Student>() != null)
            {
                Student target = collision.gameObject.GetComponent<Student>();
                bool TeamKill = false;

                if (Allies.Contains(target))
                {
                    TeamKill = true;
                }
                if (TeamKill == false)
                {
                    AudioManager.Instance.PlayAudio(impactSound);
                    if (target.currHealth > 0)
                    {
						// Hunker check
						if (target.sm.GetCurrentState() == "Hunker")
						{
							damage -= HunkerBlock;

							if (damage < 0)
							{
								damage = 0;
							}
						}

						// Damage
                        target.currHealth -= damage;

                        if (target.currHealth <= 0)
                        {
                            target.currHealth = 0;
                            FindObjectOfType<GameLogicMP>().StudentDeath(target);
                            //FindObjectOfType<GameLogic_V2>().StudentDeath(target);
                        }
                    }
                    FindObjectOfType<GameLogicMP>().SendStudentHit(gameObject, target, damage);
                    // TODO Event here...
                    active = false;
                    Destroy(gameObject);
                }
            }

            // If collided with a blocker
            if (collision.gameObject.GetComponent<ProjectileBlocker>() != null)
            {
                ProjectileBlocker blocker = collision.gameObject.GetComponent<ProjectileBlocker>();
                switch (blocker.Type)
                {
                    case ProjectileBlocker.BlockerType.BLOCK_PARTIAL:
                        {
                            float temp = Random.Range(0.0f, 100.0f);
                            if (temp < BlockChance)
                            {
                                FindObjectOfType<GameLogicMP>().SendStudentHit(gameObject);
                                // TODO Event here...
                                active = false;
                                Destroy(gameObject);
                                AudioManager.Instance.PlayAudio(blockedSound);
                            }
                            break;
                        }
                    case ProjectileBlocker.BlockerType.BLOCK_FULL:
                        {
                            FindObjectOfType<GameLogicMP>().SendStudentHit(gameObject);
                            // TODO Event here...
                            active = false;
                            Destroy(gameObject);
                            AudioManager.Instance.PlayAudio(blockedSound);
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            }
        }
    }
}