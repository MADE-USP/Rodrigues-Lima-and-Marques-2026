/******************************************************************************

	ORDER EQUATIONS
	-------------------

	Written by Lucca Rodrigues, Made-USP

 ******************************************************************************/

EQUATION("order_equations")
	/* Equation to be added in the ROOT of the model, to guarantee that the main equations are calculated in the correct order 

	This is very important in order to guarantee that all values have been updated before they are called by other equations: this can be a problem especially in case WRITE functions are used (although this problem is somewhat attenuated if EQUATION_DUMMY is used in these cases).

	Note that variables that are calculated for agents with various instances (HH or C firms) need to added here as sectoral, otherwise they are calculated only for the first agent of the category */
	
	/* V("initialization"); // model is initialized (in the first round only)
	
	if(T == 1) LOG("\n First time step started");

	VS(c_sector0, "workers_c_demand"); 
	VS(financial0, "bank_debt_supply_max");
	VS(c_sector0, "investment_demand_c"); 
	VS(k_sector0, "researchers_k_demand");
	VS(k_sector0, "workers_k_demand");
	VS(hh_sector0, "labor_employed"); 
	VS(k_sector0, "production_k");
	VS(c_sector0, "production_c");
	VS(government0, "bonds_flow");
	VS(c_sector0, "order_equations_c_sector"); // list of variables that need to be computed before exit/entry in C sector
	V("gdp_nominal");
	V("gdp_nominal_demand_approach");
	

	VS(c_sector0, "exit_c");
	VS(c_sector0, "entry_c");

	if(T == 1) LOG("\n First time step completed (some variables can still be calculated)"); */

RESULT( 0 )


EQUATION("order_equations_c_sector")
	/* Equation that guarantees that key variables in the C and other sectors are calculated BEFORE entry/exit dynamics in C sector. This equation is placed at the C sector. */
	/*
	// aggregates and statistics at C sector
		V( "production_c");
		V( "production_c_nominal");
		V( "production_c_real");
		V( "sales_c_nominal");
		V( "sales_c_real");
		V( "investment_demand_c");
		V( "investment_realized_c");
		V( "cost_investment_nominal_c");
		V( "inventories_c_real");
		V( "inventories_c_real_change");
		V( "capital_stock_c"); 
		V( "productivity_c_avg");
		V( "green_productivity_c_avg");
		V( "capacity_ut_c");
		V( "workers_c_employed");
		V( "cost_workers_c");
		V( "profits_distributed_c");
		V( "loan_c");
		V( "deposits_c");
		V( "unfilled_demand_c_average");

	// C goods market variables
		VS(PARENT, "demand_c_nominal");
		V("market_share_c_rescale");
		V("price_c_average_consumer");
		V("competitiveness_average_c");
		
	// macro aggregates
		VS(PARENT, "gdp_nominal_demand_approach");
		VS(PARENT, "gdp_nominal");
		VS(PARENT, "profits_gross_nominal");

	// bank sector variables
		VS(financial1, "bank_deposits");
		VS(financial1, "bank_loans");
		VS(financial1, "bank_net_worth");

	// government sector variables
		VS(government1, "taxes_sales");

	// HH sector
		V("cost_workers_c");
		V("labor_employed");

	// environment
		V("emissions_c");
		V("green_c");

	// K firm delivers machines
		VS(PARENT, "production_k");*/
		
RESULT( 0 )
