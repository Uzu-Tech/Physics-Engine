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

<img src="https://github.com/user-attachments/assets/6175a22f-f85c-4f9b-a37f-179e684a9a20" width="400"/>

I also added an energy loss measure on top to see roughly the error rate of the model

### Collisons with other free bodies

To support multiple balls colliding with each other, we need to implement elastic collisions between free bodies. In an elastic collision, **both momentum and kinetic energy are conserved**. In 1D, this is represented by the following equations:

**Momemtum**:  
$$m_1 u_1 + m_2 u_{2} = m_1 v_1 + m_2 v_2$$

**Energy**:  
$$\frac{1}{2} m_1 u_1^2 + \frac{1}{2} m_2 u_2^2 = \frac{1}{2} m_1 v_1^2 + \frac{1}{2} m_2 v_2^2$$

Where:
- $m_1$, $m_2$ are the masses of the free bodies
- $u_1$, $u_2$ are the initial velcoities
- $v_1$, $v_2$ are the final velocities.

Solving this equation (with a ton of alegbra) for $v_1$ and $v_2$ we get:

$v_1 = \frac{m_1 - m_2}{m_1 + m_2}u_1 + \frac{2m_2}{m_1 + m_2}u_2$

$v_2 = \frac{2m_1}{m_1 + m_2}u_1 + \frac{m_2 - m_1}{m_1 + m_2}u_2$

### Extending to 2D Collisions

These equations only work in one dimension. For 2D collisions, we project the velocities onto two axes:
- The normal axis (the direction of the collision).
- The tangent axis (perpendicular to the collision).

Velocities along the tangent axis remain unchanged—it's like two balls just brushing past each other. Only the normal components of the velocities need to be updated using the 1D formulas above. Here's the general idea in code using vector operations.

```cpp
void handleFreeBodyCollision(FreeBody& free_body1, FreeBody& free_body2) {
	// Resolve overlap by shifting the first free body out of free body 2
	float distance{ magnitude(free_body1.position - free_body2.position) };
	float overlap{
		free_body1.getRadius() + free_body2.getRadius() - distance
	};
	sf::Vector2f collision_normal{
		(free_body1.position - free_body2.position) / distance
	};
	free_body1.position += overlap * collision_normal;

	// Calculate new velocities based on conversation of momentum and kinetic energy along axis of collision
	sf::Vector2f collision_tangent(-collision_normal.y, collision_normal.x);

	// u represents the magnitude of intial velocity along the collison axis
	float u1{ dot(free_body1.velocity, collision_normal) };
	float u2{ dot(free_body2.velocity, collision_normal) };

	// Factors used in equation
	float mass_difference_factor{ (free_body1.mass - free_body2.mass) / (free_body1.mass + free_body2.mass) };
	float partner_mass_factor1{ 2 * free_body2.mass / (free_body1.mass + free_body2.mass) };
	float partner_mass_factor2{ 2 * free_body1.mass / (free_body1.mass + free_body2.mass) };

	// v represents the final velocity along the collison axis
	float v1{ mass_difference_factor * u1 + partner_mass_factor1 * u2 };
	float v2{ -mass_difference_factor * u2 + partner_mass_factor2 * u1 };

	sf::Vector2f v1_normal{ v1 * collision_normal };
	sf::Vector2f v2_normal{ v2 * collision_normal };

	sf::Vector2f v1_tangent{ dot(free_body1.velocity, collision_tangent) * collision_tangent };
	sf::Vector2f v2_tangent{ dot(free_body2.velocity, collision_tangent) * collision_tangent };

	free_body1.velocity = v1_normal + v1_tangent;
	free_body2.velocity = v2_normal + v2_tangent;
}
```

Here's the result for 5 balls:

<img src="https://github.com/user-attachments/assets/0dbba392-51cd-4447-a158-02b1d1cd0cfa" width="400"/>
