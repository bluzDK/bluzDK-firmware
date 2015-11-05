/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NRF_SEC_H__
#define NRF_SEC_H__

#include "nrf_svc.h"
#include <stdint.h>

#define NRF_SEC_SVC_BASE 0x8 /**< The lowest SVC number reserved for the nRF Security library. */

/**@brief The SVC numbers used by the SVC functions in the security library. */
enum NRF_SEC_SVCS
{
    NRF_SEC_SVC_HASH = NRF_SEC_SVC_BASE,  /**< SVC number for generating a digest using a hash function over a dataset. */
    NRF_SEC_SVC_VERIFY,                   /**< SVC number for veryfying a signature for a given dataset using provided keys and signature. */
    NRF_SEC_SVC_LAST
};

/**@brief Enumeration containing list of supported ECC curve and hash functions in the nRF Security library.
 */
typedef enum
{
    NRF_SEC_NIST256_SHA256 = 0x00, /**< Selection of NIST P-256 curve with SHA-256 digest for verification of signature. */
    NRF_SEC_ALGO_LAST              /**< Last element in enumeration closing valid algorithm list. */
}nrf_sec_algo_t;

/**@brief Enumeration containing list of supported hash functions in the nRF Security library.
 */
typedef enum
{
    NRF_SEC_SHA256 = 0x00, /**< Selection of SHA-256 hash function. */
    NRF_SEC_HASH_FUNC_LAST /**< Last element in enumeration closing valid hash function list. */
}nrf_sec_hash_func_t;

/**@brief Structure with length of data and pointer to data for signing or verification.
 */
typedef struct
{
    uint32_t   length;  /**< Length of data pointed to by @ref p_data. */
    uint8_t  * p_data;  /**< Pointer to data to be used. */
} nrf_sec_data_t;

/**@brief Structure with pointers to X and Y coordinates of a point on the curve.
 */
typedef struct
{
    uint8_t  * p_x;     /**< Pointer to X coordinate of point. */
    uint32_t   x_len;   /**< Length of data pointed to by p_x. */
    uint8_t  * p_y;     /**< Pointer to X coordinate of point. */
    uint32_t   y_len;   /**< Length of data pointed to by p_x. */
} nrf_sec_ecc_point_t;

/**@brief Structure with pointers to r and s parts of the ecc signature.
 */
typedef struct
{
    uint8_t  * p_r;     /**< Pointer to R part of signature    */
    uint32_t   r_len;   /**< Length of data pointed to by p_r  */
    uint8_t  * p_s;     /**< Pointer to S part of signature    */
    uint32_t   s_len;   /**< Length to data pointed to by p_s  */
} nrf_sec_ecc_signature_t;

/**@brief   SVC Function for verifying a signature over a given dataset.
 *
 * @param[in] p_data       Pointer to the data which must be verified.
 * @param[in] p_Q          Pointer to the public key, Q.
 * @param[in] p_signature  Pointer to the signature (r and s).
 * @param[in] algorithm    Curve and hash algorithm to be used for verifying the signature, for
                           example NIST P-256 with SHA-256.
 *
 * @retval NRF_ERROR_INVALID_DATA  If the signature does not match the dataset provided.
 * @retval NRF_ERROR_NOT_SUPPORTED If the selected algorithm is not supported in this version of 
                                   the library.
 * @retval NRF_SUCCESS             If the signature matches the dataset provided.
 */
SVCALL(NRF_SEC_SVC_VERIFY, uint32_t, nrf_sec_svc_verify (nrf_sec_data_t          * p_data,
                                                         nrf_sec_ecc_point_t     * p_Q,
                                                         nrf_sec_ecc_signature_t * p_signature,
                                                         nrf_sec_algo_t            algorithm));

/**@brief   SVC Function for generating a signature over a given dataset.
 *
 * @param[in]  p_data    Pointer to the data which must be verified.
 * @param[out] p_digest  Pointer to memory where generated digest shall be stored. Ensure 
 *                       sufficient memory is available, that is for a SHA-256 there must 
 *                       be at least 256 bits (8 words) available.
 * @param[in]  hash_func Type of fash function to use when generating the digest
 *
 * @retval NRF_ERROR_NULL          If a null pointer is provided as data or hash.
 * @retval NRF_ERROR_NOT_SUPPORTED If the selected hash functions is not supported in this version
 *                                 of the library.
 * @retval NRF_SUCCESS             If generation of a digest was successful.
 */
SVCALL(NRF_SEC_SVC_HASH, uint32_t, nrf_sec_svc_hash(nrf_sec_data_t      * p_data, 
                                                    uint8_t             * p_digest, 
                                                    nrf_sec_hash_func_t   hash_func));

#endif // NRF_SEC_H__
