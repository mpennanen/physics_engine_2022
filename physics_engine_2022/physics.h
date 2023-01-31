#pragma once
#include <limits>
#include <vector>
#include <iostream>
#include "Vec2.h"
#include "debug.h"


class Rectangle
{
public:
	bool solid;
	double width;
	double height;
	double angle;
	double gravity;
	double angular_velocity;
	double mass;
	double restitution;
	double momentOfInertia;
	Vec2 position = Vec2(0, 0);
	Vec2 velocity = Vec2(0, 0);
	std::vector<Vec2> vertex_vector;
	std::vector<Vec2> edge_vector;

	void rotate(double angle, Vec2 center)
	{
		for (auto& vertex : vertex_vector) {
			double x = vertex.x - center.x;  // Shift the vertex so that the center of rotation is at the origin
			double y = vertex.y - center.y;
			vertex.x = x * cos(angle) - y * sin(angle);  // Rotate the vertex
			vertex.y = x * sin(angle) + y * cos(angle);
			vertex.x += center.x;  // Shift the vertex back to its original position
			vertex.y += center.y;
		}
		// Recalculate the edges
		edge_vector.clear();
		for (size_t i = 0; i < vertex_vector.size(); ++i) {
			edge_vector.emplace_back(vertex_vector[(i + 1) % vertex_vector.size()] - vertex_vector[i]);
		}
	}
	void update()
	{
		if (solid == false)
		{
			velocity.y -= gravity;
			position.x += velocity.x;
			position.y += velocity.y;
			updateVertexPositions(velocity);
			angle += angular_velocity;
			rotate(angular_velocity,position);
			/*for (auto& vertex : vertex_vector) {
				vertex.x += velocity.x;
				vertex.y += velocity.y;
			}
			// Recalculate the edges
			edge_vector.clear();
			for (size_t i = 0; i < vertex_vector.size(); ++i) {
				edge_vector.emplace_back(vertex_vector[(i + 1) % vertex_vector.size()] - vertex_vector[i]);
			}*/
		}
	}
	void updateVertexPositions(Vec2 delta) {
		for (auto& vertex : vertex_vector) {
			vertex += delta;
		}
	}

	Vec2 closestPointOnEdge(Vec2 point) {
		// Initialize the minimum distance and closest point to large values
		double minDist = std::numeric_limits<double>::max();
		Vec2 closestPoint = Vec2(0,0);

		// Rotate the point about the center of the rectangle to align with the orientation of the rectangle
		//Vec2 rotatedPoint = point.rotate(position, -orientation);

		// Loop over all the edges of the rectangle
		for (const auto& edge : edge_vector) {
			// Calculate the distance from the rotated point to the edge
			//double dist = rotatedPoint.distanceToLine(edge.x, edge.y);
			double dist = point.length();
			// Update the minimum distance and closest point if necessary
			if (dist < minDist) {
				minDist = dist;
				closestPoint = point;
			}
		}

		// Rotate the closest point back to the original orientation
		return closestPoint;//.rotate(position, orientation);
	}
	// Constructor
	Rectangle(double _x, double _y, double w, double h, double _angle,double _mass, double _restitution, bool _solid)
	{
		position.x = _x;
		position.y = _y;
		angle = _angle;
		angular_velocity = 0.01;
		gravity =  0.0981;
		width = w;
		height = h;
		mass = _mass;

		momentOfInertia = mass * (width * width + height * height) / 12;

		restitution = _restitution;
		solid = _solid;
		vertex_vector = {
			{-width / 2, -height / 2},
			{width / 2, -height / 2},
			{width / 2, height / 2},
			{-width / 2, height / 2},
		};
		for (auto& vertex : vertex_vector) {
			vertex += position;  // Add the position to each vertex
		}
		edge_vector.reserve(4);
		for (size_t i = 0; i < vertex_vector.size(); ++i) {
			edge_vector.emplace_back(vertex_vector[(i + 1) % vertex_vector.size()] - vertex_vector[i]);
			std::cout << "Edge Created: " << edge_vector[edge_vector.size() - 1].x << " | " << edge_vector[edge_vector.size() - 1].y << std::endl;
		}
		rotate(angle, position);
	}
};

bool sat(Rectangle* polygonA, Rectangle* polygonB) {
	std::vector<Vec2> perpendicular_stack;

	for (auto& edge : polygonA->edge_vector) {
		if (edge.x == 0) {
			perpendicular_stack.emplace_back(1, 0);  // Use (1, 0) as the perpendicular vector
		}
		else {
			perpendicular_stack.emplace_back(-edge.y, edge.x);
		}
	}

	for (auto& edge : polygonB->edge_vector) {
		if (edge.x == 0) {
			perpendicular_stack.emplace_back(1, 0);  // Use (1, 0) as the perpendicular vector
		}
		else {
			perpendicular_stack.emplace_back(-edge.y, edge.x);
		}
	}

	for (auto& perpendicular_line : perpendicular_stack)
	{
		double amin = std::numeric_limits<double>::max();
		double amax = std::numeric_limits<double>::min();
		double bmin = std::numeric_limits<double>::max();
		double bmax = std::numeric_limits<double>::min();

		for (auto& vertex : polygonA->vertex_vector)
		{
			double dot = vertex.x * perpendicular_line.x + vertex.y * perpendicular_line.y;
			amin = std::min(amin, dot);
			amax = std::max(amax, dot);
		}

		for (auto& vertex : polygonB->vertex_vector)
		{
			double dot = vertex.x * perpendicular_line.x + vertex.y * perpendicular_line.y;
			bmin = std::min(bmin, dot);
			bmax = std::max(bmax, dot);
		}

		if (!((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))) {
			return false;
		}
	}
	return true;
}

Vec2 calculateMTV(Rectangle* rectA, Rectangle* rectB) {
	double minOverlap = std::numeric_limits<double>::max();
	Vec2 mtv = Vec2(0, 0);
	std::vector<Vec2> perpendicular_stack;
	for (auto& edge : rectA->edge_vector) {
		if (edge.x == 0) {
			perpendicular_stack.emplace_back(1, 0);  // Use (1, 0) as the perpendicular vector
		}
		else {
			perpendicular_stack.emplace_back(-edge.y, edge.x);
		}
	}

	for (auto& edge : rectB->edge_vector) {
		if (edge.x == 0) {
			perpendicular_stack.emplace_back(1, 0);  // Use (1, 0) as the perpendicular vector
		}
		else {
			perpendicular_stack.emplace_back(-edge.y, edge.x);
		}
	}

	for (auto& perpendicular_line : perpendicular_stack)
	{
		double amin = std::numeric_limits<double>::max();
		double amax = std::numeric_limits<double>::min();
		double bmin = std::numeric_limits<double>::max();
		double bmax = std::numeric_limits<double>::min();

		for (auto& vertex : rectA->vertex_vector)
		{
			double dot = vertex.x * perpendicular_line.x + vertex.y * perpendicular_line.y;
			amin = std::min(amin, dot);
			amax = std::max(amax, dot);
		}

		for (auto& vertex : rectB->vertex_vector)
		{
			double dot = vertex.x * perpendicular_line.x + vertex.y * perpendicular_line.y;
			bmin = std::min(bmin, dot);
			bmax = std::max(bmax, dot);
		}

		// Calculate overlap
		double overlap = std::min(amax - bmin, bmax - amin);
		debugDouble("Overlap", overlap);
		debugDouble("minOverlap", minOverlap);
		// Check if this is the smallest overlap found so far
		if (overlap < minOverlap) {
			minOverlap = overlap;
			mtv = perpendicular_line.normalized() * overlap;
		}
	}
	debugVec2("MTV", mtv);
	return mtv;
}

void resolveCollision(Rectangle* rect1, Rectangle* rect2) {
	// Calculate the MTV (Minimum Translation Vector)
	Vec2 mtv = calculateMTV(rect1, rect2);
	// Calculate the relative velocity of the two rectangles
	Vec2 rv = rect2->velocity - rect1->velocity;

	// Calculate the relative velocity in terms of the MTV
	double velAlongNormal = dot(rv, mtv.normalized());

	// Do not respond if velocities are separating
	if (velAlongNormal <= 0) {
		// Calculate the impulse scalar
		double e = std::min(rect1->restitution, rect2->restitution);
		double j = -(1 + e) * velAlongNormal;
		j /= 1 / rect1->mass + 1 / rect2->mass;

		// Apply impulse
		Vec2 normalMtv = mtv.normalized();
		Vec2 impulse = normalMtv * j;

		// Calculate contact points
		Vec2 contactPoint1 = rect1->closestPointOnEdge(rect2->position);
		Vec2 contactPoint2 = rect2->closestPointOnEdge(rect1->position);

		// Calculate the impulses for rotational motion
		double r1 = (contactPoint1 - rect1->position).length();
		double r2 = (contactPoint2 - rect2->position).length();
		double L1 = rect1->momentOfInertia * rect1->angular_velocity;
		double L2 = rect2->momentOfInertia * rect2->angular_velocity;
		double jr = -(1 + e) * (L1 + L2) / (1 / rect1->mass + 1 / rect2->mass + (r1 * r1) / rect1->momentOfInertia + (r2 * r2) / rect2->momentOfInertia);

		// Apply the impulses to the rectangles' velocities and angular velocities
		rect1->velocity -= impulse * (1 / rect1->mass);
		rect1->angular_velocity -= jr * r1 / rect1->momentOfInertia;

		rect2->velocity += impulse * (1 / rect2->mass);
		rect2->angular_velocity += jr * r2 / rect2->momentOfInertia;

		// Update positions
		if (rect1->solid == false)
		{
			rect1->position += mtv * 0.5;
			rect1->updateVertexPositions(mtv * 0.5);
		}

		if (rect2->solid == false)
		{
			rect2->position += mtv * -0.5;
			rect2->updateVertexPositions(mtv * -0.5);
		}

		std::cout << "j: " << j << " e: " << e << " velAlongNormal: " << velAlongNormal << std::endl;
	}
}