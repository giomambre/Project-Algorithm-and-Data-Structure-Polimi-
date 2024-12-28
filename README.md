# Pasticceria Simulation System, Grade : 30 / 30

This project is part of the final assignment for the **Algorithms and Data Structures** course (2023-2024). It implements a simulation system for an industrial pastry shop, designed to manage orders, ingredient inventory, recipes, and delivery logistics in a time-discrete environment.



## Simulation Details

- **Data Structures Used**: 
  - **Linked Lists**
  - **Red-Black Trees**
- Time-discrete simulation starting at time `t=0`.
- Commands processed at each time step:
  - Add or remove recipes.
  - Restock ingredients with quantities and expiration details.
  - Accept and queue customer orders.
  - Dispatch orders using a truck with limited capacity.

## Input Format

The program reads input from a text file containing:
1. **Initial Configuration**: Truck's periodicity and capacity.
2. **Commands**: 
   - `aggiungi_ricetta <recipe_name> <ingredient_name> <quantity> ...`  
     Adds a recipe to the catalog.
   - `rimuovi_ricetta <recipe_name>`  
     Removes a recipe if no related pending orders exist.
   - `rifornimento <ingredient_name> <quantity> <expiration> ...`  
     Restocks the ingredient inventory.
   - `ordine <recipe_name> <quantity>`  
     Places an order for a specified recipe and quantity.

## Example

Input:

5 325 aggiungi_ricetta torta farina 50 uova 10 zucchero 20 rifornimento farina 100 10 uova 100 10 zucchero 100 10 ordine torta 5


Output:

aggiunta rifornito accettato camioncino vuoto


## Technologies Used

- **Programming Language**: C
- **Data Structures**: 
  - Linked Lists for dynamic order and batch management.
  - Red-Black Trees for efficient recipe and inventory management.


## Author

**Giovanni Mambretti**  
Algorithms and Data Structures Course, Politecnico di Milano (2023-2024).
