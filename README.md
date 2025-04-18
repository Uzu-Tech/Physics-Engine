# Building a 2D Physics Engine in C++ with SFML

In this project, I set out to create a 2D physics engine that can simulate a variety of physical phenomena like kinematics, springs, collisions, electrostatic forces, and more. I used **C++17** for the code, **SFML 2.6** for rendering, and **Visual Studio** as my IDE.

## Simple Mechanics - Ball in a Box

Let’s keep it simple to start. The goal is to create a ball that has an initial velocity (which the user can control), falls under gravity, and bounces around inside a box. We’ll use SFML to draw the box and the ball, and have the ball move around inside it.

<img src="https://github.com/user-attachments/assets/9073bb6c-d941-45b5-b994-f8605efea9a0" width="400"/>

### Creating a Free Body

Right now, our ball is pretty boring – it’s just sitting there. So, to make it move, we need to give it some initial velocity and apply gravity so it starts falling.

We create a `FreeBody` class to represent the ball. This class keeps track of the ball’s position, velocity, acceleration, and mass. We'll also add a attribute called restitution that represents how much energy the ball conserves after each bounce. Let's also add some drawing functionality to display the ball.

### Updating the Position and Velocity

To update the position of the ball, we need to integrate the velocity and acceleration over a small time step $\Delta t$. There are many ways to achieve this numerically, but for now, we can use **Euler's method**.

Euler's method for updating the position and velocity is as follows:

$`
\mathbf{v}_{n+1} = \mathbf{v}_n + \mathbf{a} \Delta t
`$

$`
\mathbf{x}_{n+1} = \mathbf{x}_{n} + \mathbf{v}_{n + 1} \Delta t
`$

We can now implement this into our update method and add a apply force method to change the acceleration.

```cpp
void FreeBody::update(float delta_time)
{
	// Euler method
	velocity = velocity + delta_time * acceleration;
	position = position + delta_time * velocity;
	circle.setPosition(position);
        // Reset acceleration to apply new forces if needed
	acceleration = sf::Vector2f(0, 0);
}

void FreeBody::applyForce(sf::Vector2f force)
{
	acceleration += force / mass;
}
```

And with that we can finally get our ball to move.

<img src="https://github.com/user-attachments/assets/cdbdd892-0368-4ffe-9b8e-d917f4591c24" width="400"/>

### Dealing with Collisons

To prevent our ball from phasing through the floor let's add a simple collison detection system to detect when the ball hits the box's bottom or sides. To keep things simple let's assume the collison is elastic so the ball's kinetic energy is the same before and after. When we detect the ball is outside the box we can first resolve the collison by moving the ball back into the box and then flipping either its horizontal or vertical velocity depending on whether it hits the side, floor or ceiling.

```cpp
void handleBoxCollison(Box& box, FreeBody& free_body)
{
	// Get the positions of the floor, ceiling and sides of the box from the box coordinates with ball radius included
	float collison_ceiling{ box.pos.y + box.line_width + free_body.getRadius() };
	float collison_floor{ box.pos.y - box.line_width + box.height - free_body.getRadius() };
	float collison_left{ box.pos.x + box.line_width + free_body.getRadius() };
	float collison_right{ box.pos.x - box.line_width + box.width - free_body.getRadius() };

	// Resolve ceiling or floor collison
	if (free_body.position.y <= collison_ceiling || free_body.position.y >= collison_floor) {
		free_body.velocity.y = -free_body.velocity.y;
		free_body.position.y = (free_body.position.y <= collison_ceiling) ? collison_ceiling : collison_floor;
	}
	
	// Resolve side collison
	if (free_body.position.x <= collison_left || free_body.position.x >= collison_right) {
		free_body.velocity.x = -free_body.velocity.x;
		free_body.position.x = (free_body.position.x <= collison_left) ? collison_left : collison_right;
	}
}
```

Now we can finally get something that looks at least normal

<img src="https://github.com/user-attachments/assets/9bdbbca1-88b7-4917-b803-3e2a51f82e94" width="400"/>
