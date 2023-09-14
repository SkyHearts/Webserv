/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUnknown.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:39:25 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/14 19:20:33 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "responseBase.hpp"
#include "responseGet.hpp"

class ResponseUnknown : public ResponseBase, public ResponseGet {
	private:
		std::ifstream _file;

	public:
		ResponseUnknown( void );
		~ResponseUnknown( void );

		void generateResponse( void );

};