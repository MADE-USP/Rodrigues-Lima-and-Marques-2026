/******************************************************************************

	GOVERNMENT OBJECT EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that are specific to the public sector (government) object in the Made-Green model
	are coded below.

 ******************************************************************************/

// -----------------------------------------------------------------------------------------------------------------------====================================================================================
// PART 1: TAXES AND SPENDING ----------------------------------------------------------------------------------------------------------======================================================================================
// -----------------------------------------------------------------------------------------------------------------------=====================================================================================


	EQUATION( "government_spending_realized" ) 
		/*UNIT: monetary units

		Government expenditure (realized both consumption and unemp benefit)*/

		v[1] = V("unemployment_benefit_paid") ;// pay unemployment benefit

		v[2] =V("consumption_realized_gov") ;

		v[4] = v[2] + v[1]; // realized gov spending

	RESULT( v[4] )


	EQUATION( "consumption_realized_gov" ) 
		/*UNIT: monetary units

		Government consumption realized */

		v[2] =V("demand_consumption_government") / V("demand_c_nominal");

		v[3] = VS(c_sector1, "sales_c_nominal");

		v[4] = v[2] * v[3] ; // realized gov spending

	RESULT( v[4] )


	EQUATION( "unemployment_benefit_paid" ) 
		/*UNIT: monetary units

		Unemployment benefit */

		v[1] = VS( hh_sector1, "labor_unemployed" );// unemployed labor
		v[2] = v[1] * VS( hh_sector1, "wage_unemployed" );							// pay unemployment benefit

	RESULT( v[2] )


	EQUATION("demand_consumption_government")
		/*UNIT: monetary units
		
		Total government consumption demand (before possible supply constraint) */

		v[0] = VL("consumption_realized_gov", 1) ;
		v[1] =  VL("nominal_gdp_growth", 1);

		v[3] = (1 + v[1]) * v[0];

		v[4] = VL("gdp_nominal", 1) * V("target_gov_size");

		v[5] = max(v[3], v[4])

	RESULT( v[4] )	


	EQUATION("gov_spending_to_gdp")
		/* UNIT: ratio

		Government's spending as a ratio of GDP */

		v[0] = V("government_spending_realized");
		v[1] = V("gdp_nominal");

	RESULT( v[0] / v[1] )



	EQUATION("unemployment_benefit_to_gdp")
		/* UNIT: ratio

		Unemployment benefit as a ratio of GDP */

		v[0] = V("unemployment_benefit_paid");
		v[1] = V("gdp_nominal");

	RESULT( v[0] / v[1] )



	EQUATION("gov_consumption_to_gdp")
		/* UNIT: ratio

		Government's consumption as a ratio of GDP */

		v[0] = V("consumption_realized_gov");
		v[1] = V("gdp_nominal");

	RESULT( v[0] / v[1] )		


	EQUATION("taxes_sales")
		/* UNIT: monetary units

		Aggregate taxes received from firms based on sales */

		if( V("tax_rate_sales") == 0 ){ PARAMETER; END_EQUATION( 0 ); }

		v[0] = SUMS(c_sector1, "_taxes_sales_paid_c");
		v[1] = SUMS(k_sector1, "_taxes_sales_paid_k");

		v[2] = v[0] + v[1];

	RESULT(v[2]) 


	EQUATION("taxes_income")
		/* UNIT: monetary units

		Aggregate taxes received from HH based on their income */

		v[0] = VS(hh_sector1, "wage_taxes") + VS(hh_sector1, "dividends_taxes");

	RESULT( v[0] )

	EQUATION("wealth_tax_rate")
		/*UNIT: rate
		
		Endogenous wealth tax rate */

		v[3] = 0;

		if( V("flag_wealth_tax") == 1){

		v[1] = VLS(PARENT, "gdp_nominal", 1) * V("policy_share_output");

		v[2] = VLS(hh_sector1, "deposits_hh", 1) == 0? 0: v[1] / VLS(hh_sector1, "deposits_hh", 1);

		v[3] = min(v[2], 1);

		}
	
	RESULT( v[3] )	


	EQUATION("taxes_revenue")
		/* UNIT: monetary units

		Sum of all types of taxes collected by the government (without green taxes) */

		v[0] = V("taxes_income") + V("taxes_sales") ;

	RESULT( v[0] )	


	EQUATION("taxes_to_gdp")
		/* UNIT: ratio

		Participation of government's taxes on gdp */

		v[0] = V("taxes_revenue");
		v[1] = V("gdp_nominal");

		v[2] = v[0] / v[1];

	RESULT( v[2] )	

	EQUATION("dividends_taxes_to_gdp")
		/* UNIT: ratio

		Participation of government's taxes on gdp */

		v[0] = VS(hh_sector1, "dividends_taxes");
		v[1] = V("gdp_nominal");

		v[2] = v[0] / v[1];

	RESULT( v[2] )	


	EQUATION("gov_result_primary")
		/* UNIT: monetary units

		Government's superavit in the period (taxes - expenditures, excluding interest expenditures). This is the primary result.

		If < 0, government has a deficit */

		V("green_bonds_stock"); //make sure bonds are computed (green result updated for potential green bonds repayment)

		v[0] = V("taxes_revenue");

		v[1] = V("government_spending_realized");

		v[3] = v[0] - v[1];

	RESULT( v[3] )


	EQUATION("gov_result_primary_to_gdp")
		/* UNIT: ratio

		Government's result as a ratio of GDP */

		v[0] = V("gov_result_primary");
		v[1] = V("gdp_nominal");

	RESULT( v[0] / v[1] )

	EQUATION("gov_result_nominal")
		/* UNIT: monetary units

		Government's superavit in the period (taxes - expenditures, including interest expenditures). This is the nominal result.

		If < 0: deficit */

		v[0] = V("gov_result_primary");

		v[1] = V("bonds_interest_paid");

		v[2] = v[0] - v[1];

	RESULT( v[2] )

	EQUATION("gov_result_nominal_to_gdp")
		/* UNIT: ratio

		Government's result as a ratio of GDP */

		v[0] = V("gov_result_nominal");
		v[1] = V("gdp_nominal");

	RESULT( v[0] / v[1] )

	EQUATION("bonds_interest_paid") 
		/* UNIT: monetary units 

		Interest paid on stock of government bonds (both to CB and to bank) */

		v[0] = VL("bonds_stock", 1);
		v[1] = V("policy_interest_rate"); 

		v[2] = v[0] * v[1];

	RESULT( v[2] )	

	EQUATION("bonds_stock") 
		/* UNIT: monetary units

		Total stock of government bonds */

		v[0] = VL("bonds_stock", 1);
		v[1] = V("bonds_flow");

		v[2] = v[0] + v[1];

	RESULT( v[2] )

	EQUATION("bonds_flow")
		/* UNIT: monetary units

		Change in stock of government bonds held by private sector.

		Timeline of events (logic considered in this equation):
			government pays interest on bonds 
			government pays wages to public servants and unemployment dole and consumes
			government receives taxes on income and sales
			residual from these transactions form the government deposits at the CB */

		v[0] = V("taxes_revenue");

		v[11] = V("government_spending_realized"); // expenditure with goods and workers (in nominal terms)
		v[12] = V("bonds_interest_paid");

		v[20] = VL("gov_deposits_cb", 1); // gov deposits at the CB in the beginning of the period

		// 1st emission of bonds (acquired by the CB) -> payment of interest income and taxes on interest income

			v[30] = max(v[12] - v[20], 0); // new bill for payment of interest on bonds

			v[21] = v[20] + v[30] - v[12] ; // updated gov deposits at CB (- interest payment )

		// 2nd emission of bonds 

			v[31] = max(v[11]  - v[21], 0); // new bill for payment of unemployment dole, wages and consumption

			v[22] = v[21] + v[31] - v[11]  + v[0] ; // updated gov deposits at CB (- spending payment + taxes income and sales)

		// total emission of bonds

			v[32] = v[30] + v[31] - max( V("green_government_result"), 0);

		// gov deposits at the CB

			WRITE("gov_deposits_cb", v[22]);

	RESULT( v[32] )

	EQUATION("bonds_stock_to_gdp")
		/* UNIT: ratio

		Government's debt as a ratio of GDP */

		v[0] = V("bonds_stock");

		v[1] = V("gdp_nominal");

	RESULT( v[0] / v[1] )


	EQUATION("policy_interest_rate")
		/* UNIT: rate

		Interest rate set by the central bank, following IT regime. This option can be disabled in order to have a constant nominal interest rate */

		if( V("flag_inf_target") == 0 ) // if IT regime is disabled: interest rate becomes a parameter
		{
			PARAMETER; END_EQUATION(  VS(PARENT, "init_policy_interest_rate") );
		}

		if( T < V("it_regime_periods") ) END_EQUATION(  VL("policy_interest_rate", 1) ); // not enough info concerning inflation yet

		v[0] = VS(PARENT, "init_policy_interest_rate");

		v[1] = V("inflation_target");
		v[2] = VL("inflation_average", 1);
		v[3] = V("sensitivity_interest_inflation_gap"); // adjustment 

		v[4] = V("unemployment_rate_target");
		v[5] = VL("unemployment_rate_average", 1);
		v[6] = V("sensitivity_interest_unemployment_gap");

		v[10] = V("interest_rate_persistence");
		v[11] = VL("policy_interest_rate", 1);

		//v[20] = ( v[0] + v[2] + v[3] * ( v[2] - v[1] ) - v[6] * ( v[5] - v[4] ) ) * ( 1 - v[10] ) + ( v[10] * v[11] );

		v[20] = ( v[3] * ( v[2] - v[1] ) - v[6] * ( v[5] - v[4] ) ) * ( 1 - v[10] ) + ( v[10] * v[11] );

		v[20] = max( V("interest_rate_min") , v[20] );

	RESULT( v[20] ) 


	EQUATION( "total_bonds_stock" )
		/*UNIT: monetary units
		
		Total bonds stock (both traditional and green) */

		v[0] = V("bonds_stock") + V("green_bonds_stock");

		v[1] = 1 - V("bonds_stock") / v[0];
		WRITE("green_bonds_stock_share", v[1]);

	RESULT( v[0] )

	EQUATION_DUMMY( "green_bonds_stock_share", "green_bonds_stock_share" )


// -----------------------------------------------------------------------------------------------------------------------==================================================================================
// PART 2: GREEN PUBLIC BUDGET --------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------=====================================================================================


	EQUATION("taxes_emissions")
		/* UNIT: monetary units
		
		Aggregate taxes received from firms based on emissions from production */

		v[0] = VS(c_sector1, "emissions_c");
		v[1] = VS(k_sector1, "emissions_k");

		v[2] = (v[0] + v[1]) * V("CO2_tax");

		v[3] = v[2] / V("gdp_nominal");
		WRITE("taxes_emissions_to_gdp", v[3]);

	RESULT( v[2] )	

	EQUATION_DUMMY("taxes_emissions_to_gdp", "taxes_emissions")		

	EQUATION("CO2_tax")
		/*UNIT: rate
		
		Endogenous CO2 tax rate */

		v[2] = 0;

		if( V("flag_carbon_tax") == 1){

		v[1] = VLS(PARENT, "gdp_nominal", 1) * V("policy_share_output");

		v[2] = VLS(PARENT, "emissions", 1) == 0? 0: v[1] / VLS(PARENT, "emissions", 1);
		}
	
	RESULT(v[2] )


	EQUATION("green_government_spending")
		/* UNIT: monetary units
		
		Aggregate spending from government in its green strategies */

		v[0] = V("green_lab_cost") + V("green_price_subsidie_cost") + V("green_research_subsidie_cost") +
				V("green_basic_income_cost") + V("green_transfer_cost") + V("green_public_procurement_cost");
		
		v[3] = v[0] / V("gdp_nominal");
		WRITE("green_government_spending_to_gdp", v[3]);
		
	RESULT( v[0] )

	EQUATION_DUMMY("green_government_spending_to_gdp", "green_government_spending")

	EQUATION( "green_lab_cost" )
		/*UNIT: moentary units
		
		Active if green spending is used to fund a green lab. Hire researchers and set public innovation process. */
		
		v[0] = VL("taxes_emissions", 1) + VLS(hh_sector1, "wealth_tax", 1);

		v[3] = 0;

		v[7] = VL("green_productivity_g", 1); // current public productivity

		if( V("flag_green_lab") == 1){
			
			v[1] = v[0] / (VS( hh_sector1, "wage") * (1 + VS( hh_sector1, "wage_premium"))); //demand for public researchers

			WRITE("researchers_g_employed", v[1]);

			WRITE("research_wage_g", VS( hh_sector1, "wage") * (1 + VS( hh_sector1, "wage_premium")));

			v[3] = v[0]; //v[1] * VS( hh_sector1, "wage") * (1 + VS( hh_sector1, "wage_premium"));

			WRITE("researchers_cost_g", v[3]);		

			v[4] = VS(k_sector1, "green_innovation_easiness"); // parameter indicating how easy it is for firms to innovate

			v[5] = 1-exp(-v[4] * v[1]); // probability in success in innovating

			v[6] = bernoulli( v[5] ); // bernoulli draw

		if(v[6] == 1) // if innovated
		{

			v[8] = VS(k_sector1, "green_innovation_alpha");
			v[9] = VS(k_sector1, "green_innovation_beta");
			v[10] = VS(k_sector1, "green_innovation_inf"); // lower beta innovation draw support
			v[11] = VS(k_sector1, "green_innovation_sup"); // upper beta innovation draw support

			v[12] = beta(v[8], v[9]); // beta draw

			v[13] = v[7] * (1 - v[10] - v[12] * (v[11] - v[10]) ); //minus innovation change because we want to reduce the emissions coefficient 
			  
		}
		else // if did not innovate
		{
			v[13] = v[7];	
		}
		WRITE("green_productivity_g", v[13]);
		}
		else  WRITE("green_productivity_g", v[7]);

	RESULT( v[3] )

	EQUATION_DUMMY("researchers_g_demand", "green_lab_cost")

	EQUATION_DUMMY("researchers_g_employed", "green_lab_cost")

	EQUATION_DUMMY("researchers_cost_g", "green_lab_cost")

	EQUATION_DUMMY("green_productivity_g", "green_lab_cost")

	EQUATION_DUMMY("research_wage_g", "green_lab_cost")

	EQUATION( "green_price_subsidie_cost")
		/*UNIT: monetary units 
		
		Active if price subsidies green capital firms */

		v[0] = VL("taxes_emissions", 1) + VLS(hh_sector1, "wealth_tax", 1);

		v[1] = 0;

		if(  V("flag_green_price_subsidies") == 1){
			
			v[2] = v[0] / VS( k_sector1, "green_k");

			WRITE("green_price_subsidie", v[2]);
		}

		CYCLES(k_sector1, cur, "k_firms"){
			v[1] += VS(cur, "_green_price_subsidie_k");
   	}

	   CYCLES(c_sector1, cur, "c_firms"){
		v[1] += VS(cur, "_green_price_subsidie_c");
   }


	RESULT( v[1] )

	EQUATION_DUMMY("green_price_subsidie", "green_price_subsidie_cost")

	EQUATION( "green_research_subsidie_cost")
		/*UNIT: monetary units 
		
		Active if research subsidies to green capital firms */

		v[0] = VL("taxes_emissions", 1) + VLS(hh_sector1, "wealth_tax", 1);

		v[1] = 0;

		if(  V("flag_green_research_subsidies") == 1){
			
			v[2] = v[0] / VS( k_sector1, "total_k_firms");

			WRITE("green_research_subsidie", v[2]);

			v[1] = v[2] * VS( k_sector1, "total_k_firms");

		}

	RESULT( v[1] )

	EQUATION_DUMMY("green_research_subsidie", "green_research_subsidie_cost")

	EQUATION( "green_basic_income_cost")
		/*UNIT: monetary units 
		
		Active if research subsidies to green capital firms */

		v[0] = VL("taxes_emissions", 1) + VLS(hh_sector1, "wealth_tax", 1);

		v[1] = 0;

		if(  V("flag_green_basic_income") == 1){
			
			v[2] = v[0] / VS(hh_sector1, "labor_force");

			WRITE("green_basic_income", v[2]);

			v[1] = v[2] * VS(hh_sector1, "labor_force");

		}

	RESULT( v[1] )

	EQUATION_DUMMY("green_basic_income", "green_basic_income_cost")

	EQUATION( "green_transfer_cost")
		/*UNIT: monetary units 
		
		Active if research subsidies to green capital firms */

		v[0] = VL("taxes_emissions", 1) + VLS(hh_sector1, "wealth_tax", 1);

		v[1] = 0;

		if(  V("flag_green_transfer") == 1){
			
			v[2] = v[0] / (VS(k_sector1, "workers_k_employed") + VS(c_sector1, "workers_c_employed") + VS(hh_sector1, "labor_unemployed"));

			WRITE("green_transfer", v[2]);

			v[1] = v[2] * (VS(k_sector1, "workers_k_employed") + VS(c_sector1, "workers_c_employed") + VS(hh_sector1, "labor_unemployed"));

		}

	RESULT( v[1] )

	EQUATION_DUMMY("green_transfer", "green_transfer_cost")

	EQUATION( "green_public_procurement_cost")
		/*UNIT: monetary units 
		
		Active if research subsidies to green capital firms */

		v[0] = VL("taxes_emissions", 1) + VLS(hh_sector1, "wealth_tax", 1);

		v[1] = 0;

		/*if(  V("flag_green_public_procurement") == 1){
			
			v[2] = v[0] /  VLS(c_sector1, "green_c", 1) ; //NOMINAL

			v[3] = v[2] / VS(c_sector1, "price_c_average_before_sales"); //REAL

			WRITE("green_public_procurement_c", v[3]);

			WRITE("green_public_procurement_k", 0);

			v[1] = v[2] * VLS(c_sector1, "green_c", 1) ;

		}*/

		if(  V("flag_green_public_procurement") == 1){
			
			v[2] = v[0] /  VS(k_sector1, "green_k") ; //NOMINAL

			v[3] = v[2] / VS(k_sector1, "price_k_avg"); //REAL

			WRITE("green_public_procurement_k", v[3]);

			WRITE("green_public_procurement_c", 0);

			v[1] = v[2] * VS(k_sector1, "green_k") ;

		}

	RESULT( v[1] )

	EQUATION_DUMMY("green_public_procurement_k", "green_public_procurement_cost")

	EQUATION_DUMMY("green_public_procurement_c", "green_public_procurement_cost")

	EQUATION("green_government_result_unajusted")
		/* UNIT: monetary units
		
		Aggregate superavit from government in its green strategies.
		
		If < 0, deficit */

		v[0] = V("taxes_emissions") + VS(hh_sector1, "wealth_tax") - V("green_government_spending") - VL("green_bonds_stock",1) * VL("green_policy_interest_rate",1);

	RESULT( v[0] )	


	EQUATION("green_bonds_stock")		
		/* UNIT: monetary units
		
		Aggregate value of green bonds in the market */

		v[0] = V("green_government_result_unajusted");
		v[1] = VL("green_bonds_stock", 1); 

		if(v[0] > 0) { //if there is a superavit in green stategy

			v[2] = v[1] - v[0];		//superavit is used to repay green bonds
			v[3] = max( v[2], 0 );  
			if(v[2] > 0) 			//if all superavit is used for green bonds repayment
				WRITE("green_government_result", 0);  //superavit doesnt exists anymore
			else 
				WRITE("green_government_result", v[0] - v[3]); //if superavit is higher than green bonds stock, 
															   //the rest of superavit repays traditional debt
		}

		else 
			v[3] = v[1] - v[0];	//if there is a deficit, v[0] <0 and sum to the existing green bonds stock
			
		WRITE("green_government_result_to_gdp", v[0]/V("gdp_nominal"))

	RESULT( v[3] )

	EQUATION_DUMMY("green_government_result", "green_bonds_stock")

	EQUATION_DUMMY("green_government_result_to_gdp", "green_bonds_stock")

	EQUATION( "green_policy_interest_rate")
		/*UNIT: rate
		
		Interest rate paid by the government on green bonds*/

		v[0] = (1 - V("green_discount")) * V("policy_interest_rate");

	RESULT( v[0] )



// -----------------------------------------------------------------------------------------------------------------------====================================================================================
// DUMMY EQUATIONS  --------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------=====================================================================================


	EQUATION_DUMMY("gov_deposits_cb", "bonds_flow")






	
		
