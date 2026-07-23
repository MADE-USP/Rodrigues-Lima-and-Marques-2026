/******************************************************************************

	FINANCIAL OBJECT EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that are specific to the financial sector objects in the Made-Green model
	are coded below.

 ******************************************************************************/


// -----------------------------------------------------------------------------------------------------------------------
// PART 1: PRICING -------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

	EQUATION("interest_rate_loan") 
		/* UNIT: rate						

		Interest rate the bank receives on its assets (loans) */

		v[0] = VS(government1, "policy_interest_rate"); // base interest rate set by the central bank
		v[1] = V("interest_rate_spread"); // bank spread

		v[2] = v[0] * (1 + v[1]); // interest rate charged by the bank on loans

	RESULT( v[2] )


// -----------------------------------------------------------------------------------------------------------------------
// PART 2: BANK'S LIABILITIES AND ASSETS  -------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

	EQUATION("bank_deposits") 
		/* UNIT: monetary units

		Total of deposits held by HH and firms at the bank -> it is the banks' liability (before entry/exit) */

		// K firm

			v[0] = VS(k_sector1, "deposits_k"); 

		// C firm

			v[0] += VS(c_sector1, "deposits_c");

		// HH sector

			v[0] += VS(hh_sector1, "deposits_hh");

	RESULT( v[0] )

	EQUATION("bank_loans") 
		/* UNIT: monetary units

		Total loans of agents in the period (before entry/exit) */

		v[0] = VS(c_sector1, "loan_c") + VS(k_sector1, "loan_k");

	RESULT( v[0] )


	EQUATION( "bank_debt_supply_max" )
		/* UNIT: monetary units
				
			Bank maximum credit supply defined as the ratio between bank net worth and
			capital adequacy ratio (CAR) */

		v[0] = ( VL( "bank_net_worth", 1 ) / V( "CAR" ) ) - VL( "bank_loans", 1);	

		v[1] = max(v[0], 0);
	
	RESULT( v[1] )

	
	EQUATION("bank_gov_bonds") 
		/* UNIT: monetary units

		Stock of government bonds held by the bank */

		v[0] = VL("bank_gov_bonds", 1); // previous stock of gov bonds held by the bank

		// amount of reserves that the bank receives from the CB in the period

			//v[1] = VL("bank_gov_bonds", 1) * VLS(government1, "policy_interest_rate", 1) + VS(government1, "government_spending_realized"); // interest payment + (unemployment dole payment + government consumption)
			//v[1] = (VL("bank_gov_bonds", 1)+VLS(hh_sector1, "deposits_hh", 1)) * VLS(government1, "policy_interest_rate", 1) + VS(government1, "government_spending_realized") + VLS(government1, "green_policy_interest_rate", 1) * VL("bank_gov_green_bonds", 1) + VS(government1, "green_government_spending");
			v[1] = (VL("bank_gov_bonds", 1)) * VLS(government1, "policy_interest_rate", 1) + VS(government1, "government_spending_realized") + 
					VL("bank_gov_green_bonds", 1) *	VLS(government1, "green_policy_interest_rate", 1) + VS(government1, "green_government_spending");

		// amount of reserves that the bank transfers to the CB in the period

			//v[2] = VS(government1, "taxes_revenue") ; 
			v[2] = VS(government1, "taxes_revenue") + VS(government1, "taxes_emissions") + VS(hh_sector1, "wealth_tax") + VS(hh_sector1, "dividends_taxes"); 

		// amount of excess reserves with the bank in the end of the period

			v[3] = v[1] - v[2]; // excess reserves 

			v[4] = VL("bank_reserves_cb", 1); // previous amount of reserves at the CB

			if(v[4] < 0) // if the bank owes to the CB (reserves < 0) -> reserves are first used to pay bank's debt with CB
			{
				v[5] = min(v[3], -v[4]); // amount of debt with the CB that is repaid in the period

				v[6] = v[4] + v[5]; // new amount of bank reserves
				WRITE("bank_reserves_cb", v[6]); // updates bank reserves 

				v[10] = v[0] + max(v[3] - v[5], 0); // updates amount of gov bonds held by the bank
			}
			else // if the bank does not have a debt with the CB -> reserves are used to acquire gov bonds
			{
				v[5] = v[3] + v[4]; // available reserves
				WRITE("bank_reserves_cb", 0); // no reserves 

				v[10] = v[0] + v[5]; // updates amount of gov bonds held by the bank
			}

			WRITE("bank_gov_bonds_flow", v[10] - v[0]);

			v[11] = v[10] * VS(government1, "green_bonds_stock_share") ;

			WRITE( "bank_gov_green_bonds" , v[11] )

	RESULT( v[10] - v[11] )	

	EQUATION_DUMMY("bank_gov_green_bonds", "bank_gov_bonds")
	
	EQUATION("bank_net_worth") 
		/* UNIT: monetary units

		Bank's net worth */

		v[0] = VL("bank_net_worth",1);

		// factors that may lead to changes in the bank's net worth:

			v[2] = VL("bank_loans", 1) * VL("interest_rate_loan", 1);
			v[3] = VL("bank_gov_bonds", 1) * VLS(government1, "policy_interest_rate", 1);
			v[4] = VL("bank_gov_green_bonds", 1) * VLS(government1, "green_policy_interest_rate", 1);
			v[5] = V("bank_reserves_cb") - VL("bank_reserves_cb", 1); 
			
		v[10] = (v[2] + v[3] + v[4] + v[5] ); // change in net worth (bank has no cost)

	RESULT( v[0] + v[10] )


	EQUATION("bank_profits")
		/* UNIT: monetary unit

		Profits by the bank */

		v[0] = VL("bank_net_worth", 1);
		v[1] = V("bank_net_worth");

	RESULT( v[1] - v[0] )

// -----------------------------------------------------------------------------------------------------------------------
// DUMMY EQUATIONS  -------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

		
	EQUATION_DUMMY("bank_gov_bonds_flow", "bank_gov_bonds")	

	EQUATION_DUMMY("bank_reserves_cb",  "bank_gov_bonds")